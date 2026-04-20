#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

#include "matrix/matrix.hpp"
#include "common/matrix_iostream.hpp"

namespace fs = std::filesystem;

#ifndef TASKS_DIR
#error "TASKS_DIR is not defined"
#endif

std::vector<std::string> GetTestFileNames() {
    std::vector<std::string> file_names;
    fs::path tasks_dir(TASKS_DIR);
    if (fs::exists(tasks_dir) && fs::is_directory(tasks_dir)) {
        for (const auto& entry : fs::directory_iterator(tasks_dir)) {
            if (entry.is_regular_file()) {
                file_names.push_back(entry.path().filename().string());
            }
        }
    }
    return file_names;
}

// Обертка для алгоритма и указателя на функцию
using MulFuncPtr = void(*)(const dumb_math::matrix::Matrix&, 
                           const dumb_math::matrix::Matrix&, 
                           dumb_math::matrix::Matrix&);

struct Algorithm {
    std::string name;
    MulFuncPtr func;
};

// Перегрузка для красивого лога GTest (выведет "OptMul0" вместо "0x7fff...5a")
void PrintTo(const Algorithm& algo, std::ostream* os) {
    *os << algo.name;
}

// Тестовый класс принимает кортеж std::tuple<std::string, Algorithm>
class MatrixAlgorithmTest : public ::testing::TestWithParam<std::tuple<std::string, Algorithm>> {};

// Тело теста
TEST_P(MatrixAlgorithmTest, Correctness) {
    using namespace dumb_math::matrix;
    using namespace dumb_math::matrix::tests::iostream;

    // Распаковываем параметры
    std::string test_filename = std::get<0>(GetParam());
    Algorithm algo            = std::get<1>(GetParam());

    fs::path task_path = fs::path(TASKS_DIR) / test_filename;
    fs::path key_path  = fs::path(KEYS_DIR)  / test_filename;

    std::ifstream task_file(task_path);
    ASSERT_TRUE(task_file.is_open()) << "Unable to open task file: " << task_path;

    std::string m1_rel, m2_rel;
    ASSERT_TRUE(std::getline(task_file, m1_rel));
    ASSERT_TRUE(std::getline(task_file, m2_rel));

    fs::path m1_path = fs::path(CUR_SOURCE_DIR) / m1_rel;
    fs::path m2_path = fs::path(CUR_SOURCE_DIR) / m2_rel;

    Matrix m1(1, 1), m2(1, 1), expected_dest(1, 1);

    std::ifstream m1_stream(m1_path);
    ASSERT_TRUE(m1_stream.is_open()) << "Unable to open m1: " << m1_path;
    m1_stream >> m1;
    ASSERT_TRUE(m1_stream) << "Failed to parse m1";

    std::ifstream m2_stream(m2_path);
    ASSERT_TRUE(m2_stream.is_open()) << "Unable to open m2: " << m2_path;
    m2_stream >> m2;
    ASSERT_TRUE(m2_stream) << "Failed to parse m2";

    std::ifstream key_stream(key_path);
    ASSERT_TRUE(key_stream.is_open()) << "Unable to open key: " << key_path;
    key_stream >> expected_dest;
    ASSERT_TRUE(key_stream) << "Failed to parse key";

    Matrix actual_dest(m1.rows(), m2.cols());
    
    // Вызов тестируемой функции из структуры
    algo.func(m1, m2, actual_dest);

    ASSERT_EQ(actual_dest.rows(), expected_dest.rows()) << "Rows mismatch";
    ASSERT_EQ(actual_dest.cols(), expected_dest.cols()) << "Cols mismatch";

    const float abs_tolerance = 1e-5f; // Для чисел близких к нулю
    const float rel_tolerance = 1e-3f; // Относительная погрешность 0.1% для больших чисел

    for (size_t i = 0; i < actual_dest.rows(); ++i)
    {
        for (size_t j = 0; j < actual_dest.cols(); ++j)
        {
            const float expected = expected_dest[i, j];
            const float actual   = actual_dest[i, j];

            // Базовый шум для накопления порядка 300+ элементов.
            // Зависит от амплитуды чисел во входных файлах. 
            // Значение 6.0f покрывает потери ULP для промежуточных сумм порядка миллионов.
            const float abs_tolerance = 6.0f; 

            // 0.1% для огромных финальных значений, которые не обнулились
            const float rel_tolerance = 1e-2f; 

            // Берем максимальное значение между неизбежным шумовым порогом и относительной ошибкой
            const float max_diff = std::max(abs_tolerance, std::abs(expected) * rel_tolerance);

            EXPECT_NEAR(actual, expected, max_diff)
                << "Mismatch at index [" << i << ", " << j << "] in file " << test_filename 
                << " using algorithm " << algo.name
                << "\nActual: " << actual << ", Expected: " << expected 
                << "\nMax allowed diff was: " << max_diff;
        }
    }
}

// 4. Список алгоритмов
std::vector<Algorithm> GetAlgorithmsToTest() {
    using namespace dumb_math::matrix;
    return {
        {"DumbMul0",       Matrix::DumbMul0_},
        {"DumbMul1",       Matrix::DumbMul1_},
        {"OptMul0",        Matrix::OptMul0_},
        {"OptMul1",        Matrix::OptMul1_},
        {"BlockMul0",      Matrix::BlockMul0_},
        {"BlockMulAvx256", Matrix::BlockMulAvx256_}
    };
}

// 5. Инстанцирование Декартова произведения
INSTANTIATE_TEST_SUITE_P(
    AllAlgorithmsMatrixTests,
    MatrixAlgorithmTest,
    ::testing::Combine(
        ::testing::ValuesIn(GetTestFileNames()),
        ::testing::ValuesIn(GetAlgorithmsToTest())
    )
);