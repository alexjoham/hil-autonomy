#include "hil/vehicle.hpp"
#include "hil/angle.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <numbers>

using namespace std::numbers;

namespace {

// Roughly an RC car. Every test drives the same vehicle.
constexpr hil::VehicleParams kParams{.wheelbase = 0.25, .max_steering = 0.5};

// results should match to rounding error
constexpr double kTol = 1e-12;

}  // namespace

// ---------------------------------------------------------------------------
// derivative tests
// ---------------------------------------------------------------------------

TEST(VehicleDerivative, StandingStillHasZeroRates) {
    const hil::VehicleState state{.heading = 0.7, .speed = 0.0};
    const hil::VehicleInput input{.steering = 0.3, .acceleration = 0.0};

    const hil::VehicleState rate = hil::derivative(state, input, kParams);

    EXPECT_NEAR(rate.x, 0.0, kTol);
    EXPECT_NEAR(rate.y, 0.0, kTol);
    EXPECT_NEAR(rate.heading, 0.0, kTol);
    EXPECT_NEAR(rate.speed, 0.0, kTol);
}

TEST(VehicleDerivative, PositionRateFollowsHeading) {
    // pi/3: sin and cos clearly differ, so a swap would be caught.
    const double speed = 2.0;
    const double heading = pi / 3;
    const hil::VehicleState state{.heading = heading, .speed = speed};

    const hil::VehicleState rate = hil::derivative(state, {}, kParams);

    EXPECT_NEAR(rate.x, speed * std::cos(heading), kTol);
    EXPECT_NEAR(rate.y, speed * std::sin(heading), kTol);
}

TEST(VehicleDerivative, HeadingRateFollowsBicycleModel) {
    const double speed = 1.5;
    const double steering = 0.2;
    const hil::VehicleState state{.speed = speed};
    const hil::VehicleInput input{.steering = steering};

    const hil::VehicleState rate = hil::derivative(state, input, kParams);

    EXPECT_NEAR(rate.heading, speed / kParams.wheelbase * std::tan(steering), kTol);
}

TEST(VehicleDerivative, PositiveSteeringTurnsLeft) {
    // Convention: positive steering = left turn = counterclockwise = heading increases.
    const hil::VehicleState state{.speed = 1.0};
    const hil::VehicleInput input{.steering = 0.2};

    EXPECT_GT(hil::derivative(state, input, kParams).heading, 0.0);
}

TEST(VehicleDerivative, SteeringIsClampedAtPositiveLimit) {
    const hil::VehicleState state{.speed = 1.0};
    const hil::VehicleInput at_limit{.steering = kParams.max_steering};
    const hil::VehicleInput beyond_limit{.steering = 2 * kParams.max_steering};

    EXPECT_NEAR(hil::derivative(state, beyond_limit, kParams).heading,
                hil::derivative(state, at_limit, kParams).heading, kTol);
}

TEST(VehicleDerivative, SteeringIsClampedAtNegativeLimit) {
    const hil::VehicleState state{.speed = 1.0};
    const hil::VehicleInput at_limit{.steering = -kParams.max_steering};
    const hil::VehicleInput beyond_limit{.steering = -2 * kParams.max_steering};

    EXPECT_NEAR(hil::derivative(state, beyond_limit, kParams).heading,
                hil::derivative(state, at_limit, kParams).heading, kTol);
}

TEST(VehicleDerivative, SpeedRateEqualsAcceleration) {
    const hil::VehicleState state{.speed = 1.0};
    const hil::VehicleInput input{.acceleration = 0.8};

    EXPECT_NEAR(hil::derivative(state, input, kParams).speed, input.acceleration, kTol);
}

TEST(VehicleDerivative, ReversingWithPositiveSteeringTurnsClockwise) {
    // Like a real car backing up with the wheel turned left.
    const hil::VehicleState state{.speed = -1.0};
    const hil::VehicleInput input{.steering = 0.2};

    EXPECT_LT(hil::derivative(state, input, kParams).heading, 0.0);
}

// ---------------------------------------------------------------------------
// step_euler tests
// ---------------------------------------------------------------------------

TEST(VehicleStepEuler, OneStepAddsRateTimesDt) {
    const hil::VehicleState state{.x = 1.0, .y = -2.0, .heading = 0.4, .speed = 1.5};
    const hil::VehicleInput input{.steering = 0.2, .acceleration = 0.8};
    const double dt = 0.01;

    const hil::VehicleState rate = hil::derivative(state, input, kParams);
    const hil::VehicleState next = hil::step_euler(state, input, kParams, dt);

    EXPECT_NEAR(next.x, state.x + rate.x * dt, kTol);
    EXPECT_NEAR(next.y, state.y + rate.y * dt, kTol);
    EXPECT_NEAR(next.heading, state.heading + rate.heading * dt, kTol);
    EXPECT_NEAR(next.speed, state.speed + rate.speed * dt, kTol);
}

TEST(VehicleStepEuler, HeadingWrapsAcrossPi) {
    const hil::VehicleState state{.heading = pi - 0.01, .speed = 1.0};
    const hil::VehicleInput input{.steering = 0.3};  // turning left
    const double dt = 0.1;

    const double unwrapped =
        state.heading + hil::derivative(state, input, kParams).heading * dt;
    ASSERT_GT(unwrapped, pi) << "test setup must actually cross pi";

    const hil::VehicleState next = hil::step_euler(state, input, kParams, dt);

    // Just above -pi, not just above +pi.
    EXPECT_GE(next.heading, -pi);
    EXPECT_LT(next.heading, -pi + 0.5);
    // Compare as directions: 3.14 and -3.14 are far apart as numbers.
    EXPECT_NEAR(hil::wrap_angle(next.heading - unwrapped), 0.0, kTol);
}

TEST(VehicleStepEuler, ConstantSteeringDrivesCircle) {
    const double speed = 1.0;
    const double steering = 0.3;
    const double dt = 0.01;
    const int steps = 600;  // ~1.2 laps

    // Centre is R to the left of the start pose (start heading 0 -> +y).
    const double radius = kParams.wheelbase / std::tan(steering);
    const double cx = 0.0;
    const double cy = radius;

    hil::VehicleState state{.speed = speed};
    const hil::VehicleInput input{.steering = steering};

    for (int i = 1; i <= steps; ++i) {
        state = hil::step_euler(state, input, kParams, dt);
        const double distance = std::hypot(state.x - cx, state.y - cy);
        ASSERT_NEAR(distance, radius, speed * dt) << "at step " << i;
    }
}
