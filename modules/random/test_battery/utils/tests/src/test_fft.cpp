#include <gtest/gtest.h>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>

#include "utils/math/fft.hpp"

namespace {
    constexpr double TOLERANCE = 1e-10;

    void ExpectComplexNear(const std::complex<double>& actual, const std::complex<double>& expected)
    {
        EXPECT_NEAR(actual.real(), expected.real(), TOLERANCE);
        EXPECT_NEAR(actual.imag(), expected.imag(), TOLERANCE);
    }
}

// 1. Тест на единичный импульс (Kronecker delta)
// Вход:  {1, 0, 0, ..., 0}
// Выход: {1, 1, 1, ..., 1} (белый шум)
TEST(FFTTest, ImpulseResponse)
{
    constexpr size_t N = 8;
    std::vector<std::complex<double>> signal(N, {0.0, 0.0});
    signal[0] = {1.0, 0.0};

    rnd_generators_test::utils::math::FFT(signal);

    for (size_t i = 0; i < N; ++i)
    {
        ExpectComplexNear(signal[i], {1.0, 0.0});
    }
}

// 2. Тест на постоянную составляющую (DC Component)
// Вход:  {1, 1, 1, ..., 1}
// Выход: {N, 0, 0, ..., 0}
TEST(FFTTest, DCComponent)
{
    constexpr size_t N = 8;
    std::vector<std::complex<double>> signal(N, {1.0, 0.0});

    rnd_generators_test::utils::math::FFT(signal);

    ExpectComplexNear(signal[0], {static_cast<double>(N), 0.0});
    
    for (size_t i = 1; i < N; ++i)
    {
        ExpectComplexNear(signal[i], {0.0, 0.0});
    }
}

// 3. Тест на чистую гармонику (Sine wave)
// Вход: Синусоида с частотой k
// Выход: Два пика с амплитудой N/2 на позициях k и N-k. Остальное - нули.
TEST(FFTTest, SingleHarmonic)
{
    constexpr size_t N = 32;
    constexpr size_t target_frequency = 4;
    std::vector<std::complex<double>> signal(N);

    for (size_t n = 0; n < N; ++n)
    {
        double val = std::sin(2.0 * std::numbers::pi * target_frequency * n / N);
        signal[n] = {val, 0.0};
    }

    rnd_generators_test::utils::math::FFT(signal);

    for (size_t k = 0; k < N; ++k)
    {
        double magnitude = std::abs(signal[k]);

        if (k == target_frequency || k == N - target_frequency)
        {
            EXPECT_NEAR(magnitude, N / 2.0, TOLERANCE) << "Missing peak at bin " << k;
        }

        else
        {
            EXPECT_NEAR(magnitude, 0.0, TOLERANCE) << "Spectral leakage at bin " << k;
        }
    }
}