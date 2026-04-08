#include <gtest/gtest.h>
#include <cmath>
#include <cfenv>
#include <cerrno>
#include <limits>

#include "logarithm/logarithm.hpp"

#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif
#pragma STDC FENV_ACCESS ON
#if defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic pop
#endif

template <typename T>
class LogarithmBoundaryTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        errno = 0;
        std::feclearexcept(FE_ALL_EXCEPT);
    }
};

using FloatingPointTypes = ::testing::Types<float, double, long double>;
TYPED_TEST_SUITE(LogarithmBoundaryTest, FloatingPointTypes);


TYPED_TEST(LogarithmBoundaryTest, NegativeArgumentYieldsNaNAndDomainError) 
{
    TypeParam x = -1.0;
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isnan(result));
    EXPECT_EQ(errno, EDOM);
    EXPECT_TRUE(std::fetestexcept(FE_INVALID));
}

TYPED_TEST(LogarithmBoundaryTest, NegativeInfinityYieldsNaNAndDomainError) 
{
    TypeParam x = -std::numeric_limits<TypeParam>::infinity();
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isnan(result));
    EXPECT_EQ(errno, EDOM);
    EXPECT_TRUE(std::fetestexcept(FE_INVALID));
}

TYPED_TEST(LogarithmBoundaryTest, PositiveZeroYieldsNegativeInfinityAndPoleError) 
{
    TypeParam x = 0.0;
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isinf(result));
    EXPECT_LT(result, TypeParam(0.0)); // < 0
    EXPECT_EQ(errno, ERANGE);
    EXPECT_TRUE(std::fetestexcept(FE_DIVBYZERO));
}

TYPED_TEST(LogarithmBoundaryTest, NegativeZeroYieldsNegativeInfinityAndPoleError) 
{
    TypeParam x = -0.0;
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isinf(result));
    EXPECT_LT(result, TypeParam(0.0));
    EXPECT_EQ(errno, ERANGE);
    EXPECT_TRUE(std::fetestexcept(FE_DIVBYZERO));
}

TYPED_TEST(LogarithmBoundaryTest, OneYieldsZeroWithoutErrors) 
{
    TypeParam x = 1.0;
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_EQ(result, TypeParam(0.0));
    EXPECT_EQ(errno, 0);
    // FE_INEXACT может быть выставлен аппаратно, поэтому проверяем только отсутствие критических флагов
    EXPECT_FALSE(std::fetestexcept(FE_INVALID | FE_DIVBYZERO));
}

TYPED_TEST(LogarithmBoundaryTest, PositiveInfinityYieldsPositiveInfinity) 
{
    TypeParam x = std::numeric_limits<TypeParam>::infinity();
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isinf(result));
    EXPECT_GT(result, TypeParam(0.0));
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(std::fetestexcept(FE_INVALID | FE_DIVBYZERO));
}

TYPED_TEST(LogarithmBoundaryTest, NaNYieldsNaN) 
{
    TypeParam x = std::numeric_limits<TypeParam>::quiet_NaN();
    TypeParam result = dumb_math::logarithm::ln(x);

    EXPECT_TRUE(std::isnan(result));
    EXPECT_EQ(errno, 0);
    EXPECT_FALSE(std::fetestexcept(FE_INVALID | FE_DIVBYZERO));
}