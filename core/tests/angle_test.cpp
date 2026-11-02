#include "hil/angle.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <numbers>

using namespace std::numbers;

TEST(AngleTest, ZeroRadiansRemainZero) {
    ASSERT_EQ(hil::wrap_angle(0), 0);
}

TEST(AngleTest, PositiveThreeHalfPiWrapsToNegativeHalfPi) {
    ASSERT_EQ(hil::wrap_angle(3 * pi / 2), -pi/2);
}

TEST(AngleTest, NegativeThreeHalfPiWrapsToPositiveHalfPi) {
    ASSERT_EQ(hil::wrap_angle(-3 * pi / 2), pi/2);
}

TEST(AngleTest, SevenRadiansWrapsByOneFullTurn) {
    EXPECT_NEAR(hil::wrap_angle(7), 7 - 2 * pi, 1e-12);
}

TEST(AngleTest, FullTurnsPlusOffsetPreserveOffset) {
    EXPECT_NEAR(hil::wrap_angle(20 * pi + 0.1), 0.1, 1e-12);
}

TEST(AngleTest, PositivePiWrapsToNegativePi) {
    ASSERT_EQ(hil::wrap_angle(pi), -pi);
}

TEST(AngleTest, NaNReturnsNaN) {
    ASSERT_TRUE(std::isnan(hil::wrap_angle(NAN)));
}

TEST(AngleTest, InfinityReturnsNaN) {
    ASSERT_TRUE(std::isnan(hil::wrap_angle(std::numeric_limits<double>::infinity())));
}
