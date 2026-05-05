#include <gtest/gtest.h>
#include <immintrin.h>
#include <cstdint>

#include "engines/minstd/generator_scalar.hpp" 
#include "engines/minstd/generator_vect.hpp"

using namespace dumb_math::random::engines;

TEST(MinstdValidation, VectorSequenceMatchesScalar)
{
    const uint32_t test_seed = 12345;
    
    Minstd     scalar_engine(test_seed);
    MinstdAVX2 vector_engine(test_seed); 
    
    const int vector_iterations = 10000;

    for (int i = 0; i < vector_iterations; ++i)
    {
        __m256i v_states = vector_engine();
        
        alignas(32) uint32_t vec_results[8];
        _mm256_store_si256(reinterpret_cast<__m256i*>(vec_results), v_states);

        for (int j = 0; j < 8; ++j)
        {
            uint32_t expected_scalar = scalar_engine();
            uint32_t actual_vector   = vec_results[j];

            ASSERT_EQ(actual_vector, expected_scalar) 
                << "Sequence mismatch at global index " << (i * 8 + j) 
                << " (Iteration: " << i << ", Channel: " << j << ")";
        }
    }
}