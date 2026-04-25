# Параллелизатор генерации случайных чисел по ядрам

Функция-параллелизатор имеет следующий интерфейс

```C++

// ----- concepts.hpp -------------------
template <typename T>
concept RngParallelizable = std::uniform_random_bit_generator<T> && requires(T a, uint64_t steps) {
    { a.skipahead(steps) } -> std::same_as<void>;
};

// ------ rng_parallelizer.hpp ----------
template <concepts::RngParallelizable RngT, typename TaskFunc>
requires requires(TaskFunc task, RngT& rng, uint64_t count) {
    { task(rng, count) } -> std::default_initializable;
}
auto RngParallelRun(uint64_t total_elements, uint32_t skipahead_step, uint32_t seed, 
                    TaskFunc task, int rt_priority, std::vector<int> target_cores)

```

Функция распределяет вычисление `total_elements` элементов, каждый из которых требует `skipahead_step` вызовов генератора, по ядрам процессора. Функция `task` должна принимать ссылку на `std::uniform_random_bit_generator<T>`, у которого определён метод `skipahead()`, необходимый для установления стартовых состояний генераторов перед общим запуском, что исключает пересечение генерируемых последовательностей в разных потоках.

`rt_priority` определяет приоритет потоков на ядрах.  
Если массив ядер, на которых требуется запускать функцию `task` пуст, параллелизатор определяет набор физических ядер машины, используя информацию в `/sys/devices/system/cpu/`. При наличии более одного производительного (P-core) ядра, одно ядро резервируется под нужды операционной системы, а остальные выделяются под выполнение task.

Общее количество элементов `total_elements` распределяется поровну по ядрам, и на каждом ядре запускается `task` с аргументом, равным размеру выделенного ему сегмента элементов.


Параллелизатор использует аппаратную привязку потоков к логическому ядру с помощью `pthread_setaffinity_np`. На каждом физическом ядре на одном логическом ядре выполняется ровно один поток. Это исключает миграцию потоков по физическим и логическим ядрам, влекущую значительные накладные расходы на инвалидацию кешей и смену контекста ядер. 


## Валидация
Тесты находятся в [tests/validation](tests/validation). Тестируется верное распределение по потокам: отсутствие пересечений чанков, обработка всех элементов.

Для создания модульного тестирование, тестирование проводится для генератора-заглушки и функции-заглушки:

* _концепт генератора, поддерживающего метод skipahead_
```cpp

template <typename T>
concept RngParallelizable = std::uniform_random_bit_generator<T> && requires(T a, uint64_t steps) {
    { a.skipahead(steps) } -> std::same_as<void>;
};
```

* _заглушка_
```cpp
struct DummyRng
{
    explicit DummyRng(uint32_t seed) : seed(seed), offset(0) {}
    
    uint32_t seed = 0;
    uint64_t offset = 0;
    uint64_t operator()() { return offset++; }
    
    void skipahead(uint64_t steps) { offset += steps; }
    
    static constexpr uint64_t min() { return 0; }
    static constexpr uint64_t max() { return UINT64_MAX; }
};

static_assert(concepts::RngParallelizable<DummyRng>, "DummyRng does not satisfy RngParallelizable concept!");

struct TaskExecutionRecord
{
    uint64_t received_count;
    uint64_t received_offset;
};

struct DummyTask
{
    TaskExecutionRecord operator()(DummyRng& rng, uint64_t count) const {
        return {count, rng.offset};
    }
};
```

Тест через `std::thread::hardware_concurrency()` определяет количество логических процессоров, видимых операционной системе в текущем контексте исполнения, и запускает `RngParallelRun`, жестко задавая набор логических ядер. Тест проводит запуски для набора количества элементов от 0 до 100'000.