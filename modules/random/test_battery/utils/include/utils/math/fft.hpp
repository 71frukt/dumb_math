#pragma once

#include <concepts>
#include <vector>
#include <complex>

namespace rnd_generators_test::utils::math {

    template <std::floating_point T>
    void FFT(std::vector<std::complex<T>>& a)
    {
        size_t n = a.size();
        if (n <= 1) return;

        std::vector<std::complex<T>> a0(n / 2), a1(n / 2);
        for (size_t i = 0; i * 2 < n; ++i) {
            a0[i] = a[i * 2];
            a1[i] = a[i * 2 + 1];
        }

        FFT(a0);
        FFT(a1);

        T angle = 2 * M_PI / n;
        std::complex<T> w(1), wn(std::cos(angle), std::sin(angle));
        for (size_t i = 0; i * 2 < n; ++i) {
            a[i] = a0[i] + w * a1[i];
            a[i + n / 2] = a0[i] - w * a1[i];
            w *= wn;
        }
    }

} // namespace rnd_generators_test::math::fft