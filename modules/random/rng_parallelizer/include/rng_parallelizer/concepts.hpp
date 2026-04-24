#pragma once

#include <random>
#include <immintrin.h>

namespace dumb_math::random::concepts {

template <typename T>
concept RngParallelizable = requires(T a, uint64_t steps) {
    { a.skipahead(steps) } -> std::same_as<void>;
};


template <typename T>
concept ScalarRngType = std::uniform_random_bit_generator<T> &&
                        RngParallelizable<T>;

                        
template <typename T>
concept SimdRngType = RngParallelizable<T> && requires(T a) {
    typename T::result_t;
    requires std::is_same_v<typename T::result_t, __m256i>;

    { T::min() } -> std::same_as<uint32_t>;
    { T::max() } -> std::same_as<uint32_t>;

    { a() }; 
    requires std::is_same_v<decltype(a()), __m256i>;
};

} // namespace dumb_math::random::concepts