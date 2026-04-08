#pragma once

#include <cstdint>
#include <array>
#include <random>

#include "RLogSU/logger.hpp"

namespace dumb_math::random {

class MRG32k3a {
public:
    explicit MRG32k3a(uint32_t seed = 12345)
    {
        if (seed == 0)
        {
            RLSU_WARNING("seed = 0 in MRG32k3a ctor");
            seed = 1;
        }
        
        x_ = {seed, seed, seed};
        y_ = {seed, seed, seed};
    }

public:
    // for std::uniform_random_bit_generator concept
    using result_t = uint32_t;

    static constexpr int64_t M1 = 4294967087LL;
    static constexpr int64_t M2 = 4294944443LL;

public:

    static constexpr result_t min() { return 1; }
    static constexpr result_t max() { return static_cast<result_t>(M1 - 1); }

    result_t operator()()
    {
        int64_t p1 = (1403580LL * x_[1] - 810728LL * x_[0]) % M1;
        if (p1 < 0) p1 += M1;
        
        x_[0] = x_[1];
        x_[1] = x_[2];
        x_[2] = p1;

        int64_t p2 = (527612LL * y_[2] - 1370589LL * y_[0]) % M2;
        if (p2 < 0) p2 += M2;

        y_[0] = y_[1];
        y_[1] = y_[2];
        y_[2] = p2;

        int64_t z = x_[2] - y_[2];
        if (z <= 0) z += M1;

        return static_cast<result_t>(z);
    }

private:
    std::array<int64_t, 3> x_;
    std::array<int64_t, 3> y_;

};

}