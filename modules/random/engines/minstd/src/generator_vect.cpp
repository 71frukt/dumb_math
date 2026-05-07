#include <cstdint>
#include <immintrin.h>

#include "engines/minstd/tools/mersenne_mod_exp.hpp"
#include "engines/minstd/generator_vect.hpp"

namespace dumb_math::random::engines {

MinstdAVX2::MinstdAVX2(uint32_t seed)
{
    uint64_t base_state = (seed == 0 || seed == tools::MERSENNE_NUM) ? 1 : tools::MersenneMod(seed);
    base_state = tools::MersenneMod(base_state * a_);

    alignas(32) uint32_t states[8];
    for (int i = 0; i < 8; ++i)
    {
        states[i] = static_cast<uint32_t>(base_state);
        base_state = tools::MersenneMod(base_state * a_);
    }
    
    state_vec_ = _mm256_load_si256(reinterpret_cast<const __m256i*>(states));

    // A^8 mod M
    uint64_t jump_mult = tools::FastMersenneModExp(a_, 8);
    a_vec_ = _mm256_set1_epi32(static_cast<uint32_t>(jump_mult));
}


MinstdAVX2::result_t MinstdAVX2::operator()()
{
    __m256i current_state = state_vec_;
    state_vec_ = StateMulMod(state_vec_, a_vec_);
    return current_state;
}

void MinstdAVX2::skipahead(uint64_t offset)
{
    uint64_t jump_mult = tools::FastMersenneModExp(a_, offset * 8);
    __m256i mult_vec = _mm256_set1_epi32(static_cast<uint32_t>(jump_mult));
    
    state_vec_ = StateMulMod(state_vec_, mult_vec);
}

__m256i MinstdAVX2::StateMulMod(__m256i state, __m256i mult)
{
    // 32-bit * 32-bit = 64-bit
    // каналы 0, 2, 4, 6
    __m256i even_64 = _mm256_mul_epu32(state, mult);
    
    // нечетные каналы сдвигаются на четные позиции перед умножением
    __m256i state_odd = _mm256_srli_epi64(state, 32);
    __m256i odd_64    = _mm256_mul_epu32(state_odd, mult);

    __m256i mask_mersenne_num = _mm256_set1_epi64x(tools::MERSENNE_NUM);
    
    auto reduce = [&](__m256i x) -> __m256i {
        __m256i low   = _mm256_and_si256(x, mask_mersenne_num);     // x &= mask_mersenne_num
        __m256i high  = _mm256_srli_epi64(x, 31);               // x >>= 31
        __m256i sum   = _mm256_add_epi64(low, high);
        
        __m256i low2  = _mm256_and_si256(sum, mask_mersenne_num);
        __m256i high2 = _mm256_srli_epi64(sum, 31);
        return _mm256_add_epi64(low2, high2);
    };

    even_64 = reduce(even_64);
    odd_64  = reduce(odd_64);

    odd_64 = _mm256_slli_epi64(odd_64, 32);
    
    return _mm256_or_si256(even_64, odd_64);
}

} // namespace dumb_math::random::engines