#pragma once

#include <random>
#include "RLogSU/logger.hpp"

namespace dumb_math::random::common {

template <std::uniform_random_bit_generator generatorT, typename CalcT>
CalcT GenerateInSection(generatorT engine, CalcT left_bound, CalcT right_bound)
{
    RLSU_ASSERT(left_bound < right_bound);

    double d_left = static_cast<double>(left_bound);
    double d_right = static_cast<double>(right_bound);

    double normalized = static_cast<double>(engine() - generatorT::min()) / static_cast<double>(generatorT::max() - generatorT::min());
    
    double generated = d_left + normalized * (d_right - d_left);

    return static_cast<CalcT>(generated);
}

} // namespace dumb_math::random::common