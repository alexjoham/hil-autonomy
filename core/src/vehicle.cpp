#include "hil/vehicle.hpp"
#include "hil/angle.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

using namespace hil;

VehicleState hil::derivative(const VehicleState& s, const VehicleInput& u, const VehicleParams& p) {
    assert(p.wheelbase > 0);
    assert(p.max_steering >= 0);
    const double steering_clamped = std::clamp(u.steering, -p.max_steering, p.max_steering);

    const double x_dot = s.speed * std::cos(s.heading);
    const double y_dot = s.speed * std::sin(s.heading);
    const double heading_dot = (s.speed / p.wheelbase) * std::tan(steering_clamped);
    const double speed_dot = u.acceleration;
    return VehicleState{.x = x_dot, .y = y_dot, .heading = heading_dot, .speed = speed_dot};
}

VehicleState hil::step_euler(const VehicleState& s, const VehicleInput& u, const VehicleParams& p, double dt) {
    const VehicleState rate = derivative(s, u, p);

    const double x_new = s.x + rate.x * dt;
    const double y_new = s.y + rate.y * dt;
    const double heading_new = wrap_angle(s.heading + rate.heading * dt);
    const double speed_new = s.speed + rate.speed * dt;
    
    return VehicleState{.x = x_new, .y = y_new, .heading = heading_new, .speed = speed_new};
}
