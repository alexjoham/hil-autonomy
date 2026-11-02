#include "hil/angle.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <numbers>

using namespace std::numbers;

TEST(AngleTest, ZeroRadiansRemainZero) {
    EXPECT_EQ(hil::wrap_angle(0), 0);
}

TEST(AngleTest, PositiveThreePiWrapsToNegativePi) {
    EXPECT_EQ(hil::wrap_angle(3 * pi), -pi);
}

TEST(AngleTest, PositiveThreeHalfPiWrapsToNegativeHalfPi) {
    EXPECT_EQ(hil::wrap_angle(3 * pi / 2), -pi/2);
}

TEST(AngleTest, NegativeThreeHalfPiWrapsToPositiveHalfPi) {
    EXPECT_EQ(hil::wrap_angle(-3 * pi / 2), pi/2);
}

TEST(AngleTest, SevenRadiansWrapsByOneFullTurn) {
    EXPECT_NEAR(hil::wrap_angle(7), 7 - 2 * pi, 1e-12);
}

TEST(AngleTest, FullTurnsPlusOffsetPreserveOffset) {
    EXPECT_NEAR(hil::wrap_angle(20 * pi + 0.1), 0.1, 1e-12);
}

TEST(AngleTest, PositivePiWrapsToNegativePi) {
    EXPECT_EQ(hil::wrap_angle(pi), -pi);
}

TEST(AngleTest, NegativePiWrapsToNegativePi) {
    EXPECT_EQ(hil::wrap_angle(-pi), -pi);
}

TEST(AngleTest, LargestDoubleBelowPiReturnsUnchanged) {
    EXPECT_EQ(hil::wrap_angle(std::nextafter(pi, 0.0)), std::nextafter(pi, 0.0));
}

TEST(AngleTest, NaNReturnsNaN) {
    EXPECT_TRUE(std::isnan(hil::wrap_angle(std::numeric_limits<double>::quiet_NaN())));
}

TEST(AngleTest, InfinityReturnsNaN) {
    EXPECT_TRUE(std::isnan(hil::wrap_angle(std::numeric_limits<double>::infinity())));
}
