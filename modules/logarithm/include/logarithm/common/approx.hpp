#include "RLogSU/logger.hpp"
#include <type_traits>


namespace {

    template <size_t N, typename T>
    constexpr std::array<T, N> GenerateArtanhCoeffs()
    {
        std::array<T, N> coeffs{};
        for (size_t i = 0; i < N; ++i)
        {
            coeffs[i] = T(1.0) / static_cast<T>(2 * i + 1);
        }
        return coeffs;
    }

} // namespace

namespace dumb_math {
namespace common    {

template <std::floating_point T>
T LnArtgTailorBestAcc_0to2(T x)
{
    RLSU_ASSERT((x <= 2) && (x >= 0), "value (x = {}) outside the segment [0; 2]", x);

    T y0    = (x - 1) / (x + 1);
    T yi    = y0;
    T y0_sq = y0 * y0;
    // ln(x) = 2 * sum{n = 0, inf} (y^(2n + 1) / (2n + 1))

    T raw      = y0;
    T prev_raw = 0;

    for (size_t k = 1; raw != prev_raw; k++)    // until the next member of the row becomes less than ULP
    {
        prev_raw = raw;

        yi *= y0_sq;
        raw += yi / (2 * k + 1);
    }

    return raw * 2;
}


template <size_t N, std::floating_point T>
T LnArtgTailor_0to2(T x)
{
    T y = (x - T(1.0)) / (x + T(1.0));
    T y_sq = y * y;
    
    T row = T(0.0);
    T y_pow = y;

    static constexpr std::array<T, N> coeffs = ::GenerateArtanhCoeffs<N, T>();

    for (size_t i = 0; i < N; ++i)
    {
        row += y_pow * coeffs[i]; 
        y_pow *= y_sq;
    }

    return row * T(2.0);
}

} // namespace common
} // namespace dumb_math