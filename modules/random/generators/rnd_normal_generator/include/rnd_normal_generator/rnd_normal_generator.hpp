#pragma once

#include <concepts>
#include <random>
#include <limits>

namespace dumb_math::random {

class NormalGenerator
{
public:
    NormalGenerator() = default;
    
    template <std::uniform_random_bit_generator generatorT, std::floating_point FloatingT = double>
    FloatingT operator()(generatorT& generator)
    {
        if (has_spare_)
        {
            has_spare_ = false;
            return spare_value_;
        }

        double point_x = 0;
        double point_y = 0;
        double radius  = 0;

        do
        {
            point_x = 2.0 * ToCanonicalStrict(generator) - 1.0;
            point_y = 2.0 * ToCanonicalStrict(generator) - 1.0;

            radius  = point_x * point_x + point_y * point_y;

        } while (radius >= 1.0 || radius == 0.0);

        double phi_radius = std::sqrt(-2.0 * std::log(radius) / radius);

        spare_value_ = point_y * phi_radius;
        has_spare_   = true;

        return         point_x * phi_radius;
    }

    using result_t = uint32_t;

    static constexpr double min() { return std::numeric_limits<double>::min(); }
    static constexpr double max() { return std::numeric_limits<double>::max(); }


private:
    bool   has_spare_   = false;
    double spare_value_ = 0.0;

public:
    // makes generator value in (0, 1)
    template <std::uniform_random_bit_generator generatorT, std::floating_point FloatingT = double>
    static FloatingT ToCanonicalStrict(generatorT& generator)
    {
        constexpr FloatingT min_val = static_cast<FloatingT>(generatorT::min());
        constexpr FloatingT max_val = static_cast<FloatingT>(generatorT::max());
                  FloatingT res     = static_cast<FloatingT>(generator());
        
        return (res - min_val + 0.5) / (max_val - min_val + 1.0); 
    }
};


} // namespace dumb_math::random