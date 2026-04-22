#include <cstdint>
#include <vector>

namespace rnd_generators_test::utils {

// Функция берет генератор и выдает вектор битов, смещенных в {-1.0, 1.0}
template<typename Generator>
std::vector<double> GenerateBitSequence(Generator& engine, size_t num_bits)
{
    std::vector<double> bit_seq;
    bit_seq.reserve(num_bits);

    size_t bits_collected = 0;
    while (bits_collected < num_bits)
    {
        uint32_t val = engine(); // Получаем сырые 32 бита
        
        // Разбираем число на 32 бита (от младшего к старшему)
        for (int b = 0; b < 32 && bits_collected < num_bits; ++b)
        {
            double bit_val = ((val >> b) & 1) ? 1.0 : -1.0;
            bit_seq.push_back(bit_val);
            bits_collected++;
        }
    }
    return bit_seq;
}

} // namespace rnd_generators_test::utils