#pragma once

#include <random>

namespace dumb_math::random::concepts {

template <typename T>
concept RngType = std::uniform_random_bit_generator<T> && requires(T a, uint64_t steps) {
    { a.skipahead(steps) } -> std::same_as<void>;
};

} // namespace dumb_math::random::concepts