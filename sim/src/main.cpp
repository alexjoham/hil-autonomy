#include "hil/vehicle.hpp"
#include <cmath>
#include <format>
#include <iostream>
#include <string>

using namespace hil;

int main() {
    const double speed = 1.0;
    const double steering = 0.3;
    const double dt = 0.01;
    const double duration = 20;

    const VehicleParams params{.wheelbase = 0.25, .max_steering = 0.5};

    const VehicleInput vehicleInput{.steering = steering};

    VehicleState state{.speed = speed};

    const std::string header = std::format("t,x,y,heading,speed\n");

    std::cout << header;

    double steps = static_cast<int>(std::lround(duration/dt));
    for (int i = 0; i < steps; i++) {
        double t = i * dt;
        const std::string step = std::format("{},{},{},{},{}\n", t, state.x, state.y, state.heading, state.speed);
        std::cout << step;
        if (t >= duration) break;

        state = step_euler(state, vehicleInput, params, dt);
    }
}
