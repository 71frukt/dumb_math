#pragma once

#include <compare>
#include <vector>

namespace rnd_generators_test::utils {

template <std::ranges::input_range Range>
bool BoundsCheckStrict(const Range& data, 
                       const std::ranges::range_value_t<Range>& left_border, 
                       const std::ranges::range_value_t<Range>& right_border)
requires std::three_way_comparable<std::ranges::range_value_t<Range>>
{
    for (const auto& item : data)
    {
        if (item < left_border || item > right_border)
            return false;
    }

    return true;
}

} // namespace rnd_generators_test::utils