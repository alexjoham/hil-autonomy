#include "hil/vehicle.hpp"
#include <format>
#include <iostream>
#include <string>

using namespace hil;

int main() {
    const double speed = 1.0;
    const double steering = 0.3;
    const double dt = 0.01;
    const double duration = 20;

    VehicleParams params{.wheelbase = 0.25, .max_steering = 0.5};

    VehicleInput vehicleInput{.steering = steering};

    hil::VehicleState state{.speed = speed};

    std::string header = std::format("t,x,y,heading,speed\n");

    std::cout << header;

    double steps = duration/dt;
    for (int i = 0; i < steps; i++) {
        double t = i * dt;
        if (t >= 20) break;

        std::string step = std::format("{},{},{},{},{}\n", t, state.x, state.y, state.heading, state.speed);
        std::cout << step;

        state = hil::step_euler(state, vehicleInput, params, dt);
    }
}
