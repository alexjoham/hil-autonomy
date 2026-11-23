#include "hil/track.hpp"

#include "hil/geometry.hpp"

#include <array>
#include <cmath>
#include <numbers>

#include <gtest/gtest.h>

namespace {

constexpr double tolerance = 1e-12;
constexpr hil::Point center{.x = 2.0, .y = -1.0};
constexpr double radius = 3.0;
constexpr std::size_t count = 16;

// The test circle, shared by all tests: 16 points, off-origin centre so that a
// forgotten centre offset cannot pass by accident.
std::array<hil::Point, count> make_circle()
{
    std::array<hil::Point, count> points{};
    hil::fill_circle(points, center, radius);
    return points;
}

} // namespace

TEST(FillCircle, AllPointsLieOnTheCircle)
{
    const auto points = make_circle();
    for (std::size_t i = 0; i < points.size(); ++i) {
        EXPECT_NEAR(hil::distance(points[i], center), radius, tolerance) << "point " << i;
    }
}

TEST(FillCircle, FirstPointIsRightOfCenter)
{
    const auto points = make_circle();
    EXPECT_NEAR(points[0].x, center.x + radius, tolerance);
    EXPECT_NEAR(points[0].y, center.y, tolerance);
}

TEST(FillCircle, RunsCounterclockwise)
{
    const auto points = make_circle();
    EXPECT_GT(points[1].y, points[0].y);
}

TEST(FillCircle, NeighboursAreEquallySpacedIncludingWrapAround)
{
    const auto points = make_circle();
    const double expected = 2.0 * radius * std::sin(std::numbers::pi / static_cast<double>(count));
    for (std::size_t i = 0; i < points.size(); ++i) {
        // (i + 1) % size closes the loop: the last gap, from the last point back
        // to the first, must be as long as all others. This catches a fencepost
        // error that places the last point on top of the first.
        const hil::Point& next = points[(i + 1) % points.size()];
        EXPECT_NEAR(hil::distance(points[i], next), expected, tolerance) << "gap after point " << i;
    }
}

TEST(NearestIndex, PositionOnAPointReturnsThatPoint)
{
    const auto points = make_circle();
    for (std::size_t i = 0; i < points.size(); ++i) {
        EXPECT_EQ(hil::nearest_index(points, points[i]), i);
    }
}

TEST(NearestIndex, PositionSlightlyOffAPointReturnsThatPoint)
{
    const auto points = make_circle();
    const hil::Point near_five{.x = points[5].x + 0.05, .y = points[5].y - 0.03};
    EXPECT_EQ(hil::nearest_index(points, near_five), 5U);
}

TEST(NearestIndex, PositionNearLastPointReturnsLastIndex)
{
    const auto points = make_circle();
    const hil::Point near_last{.x = points[count - 1].x + 0.01, .y = points[count - 1].y};
    EXPECT_EQ(hil::nearest_index(points, near_last), count - 1);
}

TEST(NearestIndex, TieReturnsFirstIndex)
{
    // Exactly equal distances need exactly representable coordinates. Points
    // from fill_circle are rounded by cos and sin, so their distances to the
    // centre differ in the last bits and are not a real tie.
    const std::array<hil::Point, 2> pair{hil::Point{.x = 1.0, .y = 0.0}, hil::Point{.x = -1.0, .y = 0.0}};
    EXPECT_EQ(hil::nearest_index(pair, hil::Point{}), 0U);
}

TEST(NearestIndex, SinglePointTrack)
{
    const std::array<hil::Point, 1> one{hil::Point{.x = 4.0, .y = 4.0}};
    EXPECT_EQ(hil::nearest_index(one, hil::Point{}), 0U);
}
