#include "hil/pure_pursuit.hpp"

#include "hil/geometry.hpp"
#include "hil/track.hpp"
#include "hil/vehicle.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <numbers>

#include <gtest/gtest.h>

namespace {

constexpr hil::VehicleParams car{.wheelbase = 0.25, .max_steering = 0.5};
constexpr double lookahead = 0.5;

// 20 points spaced 0.1 m apart along y = offset, from x = 0 to x = 1.9.
std::array<hil::Point, 20> straight_line(double offset)
{
    std::array<hil::Point, 20> points{};
    for (std::size_t i = 0; i < points.size(); ++i) {
        points[i] = hil::Point{.x = 0.1 * static_cast<double>(i), .y = offset};
    }
    return points;
}

/* A car standing on the given circle point, pointing along the counterclockwise tangent.
For a point at angle theta around the centre, that is theta + pi/2.
*/
hil::VehicleState on_circle(hil::Point center, hil::Point p)
{
    const double theta = std::atan2(p.y - center.y, p.x - center.x);
    return hil::VehicleState{.x = p.x, .y = p.y, .heading = theta + std::numbers::pi / 2.0, .speed = 1.0};
}

} // namespace

TEST(PurePursuit, AlignedOnStraightLineSteersStraight)
{
    const auto track = straight_line(0.0);
    EXPECT_NEAR(hil::pure_pursuit(hil::VehicleState{}, track, car, lookahead), 0.0, 1e-12);
}

TEST(PurePursuit, TrackToTheLeftSteersLeft)
{
    const auto track = straight_line(0.3);
    EXPECT_GT(hil::pure_pursuit(hil::VehicleState{}, track, car, lookahead), 0.0);
}

TEST(PurePursuit, TrackToTheRightSteersRight)
{
    const auto track = straight_line(-0.3);
    EXPECT_LT(hil::pure_pursuit(hil::VehicleState{}, track, car, lookahead), 0.0);
}

TEST(PurePursuit, OnCircleSteersLikeTheCircle)
{
    /* A car driving exactly on a circle of radius R needs steering atan(L / R).
    Not exact, because the target is a discrete track point.
    */
    constexpr hil::Point center{.x = 1.0, .y = -2.0};
    constexpr double radius = 2.0;
    std::array<hil::Point, 200> track{};
    hil::fill_circle(track, center, radius);

    const double expected = std::atan(car.wheelbase / radius);
    EXPECT_NEAR(hil::pure_pursuit(on_circle(center, track[0]), track, car, lookahead), expected, 0.005);
}

TEST(PurePursuit, WrapsAroundAtTheEndOfTheTrack)
{
    /* The car stands on the last track point, so the target lies after the wrap to index 0.
    If the search stopped at the end of the array, the result would differ clearly from the circle steering.
    */
    constexpr hil::Point center{};
    constexpr double radius = 2.0;
    std::array<hil::Point, 200> track{};
    hil::fill_circle(track, center, radius);

    const double expected = std::atan(car.wheelbase / radius);
    EXPECT_NEAR(hil::pure_pursuit(on_circle(center, track.back()), track, car, lookahead), expected, 0.005);
}

TEST(PurePursuit, NeverExceedsSteeringLimit)
{
    std::array<hil::Point, 100> track{};
    hil::fill_circle(track, hil::Point{}, 2.0);

    for (int ix = -6; ix <= 6; ++ix) {
        for (int iy = -6; iy <= 6; ++iy) {
            for (int ih = 0; ih < 16; ++ih) {
                const hil::VehicleState state{.x = 0.5 * ix, .y = 0.5 * iy, .heading = std::numbers::pi / 8.0 * ih};
                const double steering = hil::pure_pursuit(state, track, car, lookahead);
                ASSERT_TRUE(std::isfinite(steering)) << ix << ' ' << iy << ' ' << ih;
                ASSERT_LE(std::abs(steering), car.max_steering) << ix << ' ' << iy << ' ' << ih;
            }
        }
    }
}

TEST(PurePursuit, TrackShorterThanLookaheadStaysFinite)
{
    /* No track point is lookahead away.
    The search must end (a hang here would time out the test) and still return a usable steering angle.
    */
    std::array<hil::Point, 16> tiny{};
    hil::fill_circle(tiny, hil::Point{}, 0.1);
    const double steering = hil::pure_pursuit(on_circle(hil::Point{}, tiny[0]), tiny, car, lookahead);
    EXPECT_TRUE(std::isfinite(steering));
    EXPECT_LE(std::abs(steering), car.max_steering);
}

/* With the target exactly behind, alpha is +-pi, sin(alpha) is 0, and pure pursuit steers straight away from the track
Remove DISABLED_ once pure_pursuit steers at full lock for targets behind the car.
*/
TEST(PurePursuit, DISABLED_TargetBehindSteersFullLock)
{
    const auto track = straight_line(0.0);
    const hil::VehicleState facing_away{.x = 0.0, .y = 0.0, .heading = std::numbers::pi};
    EXPECT_NEAR(std::abs(hil::pure_pursuit(facing_away, track, car, lookahead)), car.max_steering, 1e-12);
}
