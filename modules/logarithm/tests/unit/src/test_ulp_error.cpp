#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <string_view>

#include "statistics/generated/config.hpp"

#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#pragma STDC FENV_ACCESS ON
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif

using namespace dumb_math::logarithm::tests;

namespace {
    void VerifyUlpErrorLimit(std::string_view base_filename, double max_allowed_ulp)
    {
        std::string path = std::string(statistics::config::DATA_DIR) + "/" + std::string(base_filename) + ".txt";
        std::ifstream file(path);

        ASSERT_TRUE(file.is_open()) 
            << "Cannot open file: " << path << "\n"
            << "Ensure that statistics data generation ran before unit tests.";

        double x = 0.0;
        double ulp_error = 0.0;
        size_t line_idx = 1;

        while (file >> x >> ulp_error)
        {
            ASSERT_LT(ulp_error, max_allowed_ulp) 
                << "ULP error exceeded limit in file: " << base_filename << "\n"
                << "Line: " << line_idx << "\n"
                << "x = " << x << "\n"
                << "Actual ULP error = " << ulp_error << "\n"
                << "Max allowed = " << max_allowed_ulp;
            ++line_idx;
        }
    }
}

TEST(UlpErrorFileIntegrationTest, Float1toInfErrorBelow3) {
    VerifyUlpErrorLimit(statistics::config::FILE_1TOINF_FLOAT, 3.0);
}

TEST(UlpErrorFileIntegrationTest, FloatNear1ErrorBelow3) {
    VerifyUlpErrorLimit(statistics::config::FILE_NEAR1_FLOAT, 3.0);
}

