#include "hil/pure_pursuit.hpp"
#include "hil/track.hpp"
#include "hil/vehicle.hpp"

#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <numbers>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "usage: hil_sim <track.csv>\n";
        return 1;
    }

    const double dt = 0.01;
    const double duration = 20.0;
    const double lookahead = 0.5;
    const hil::VehicleParams params{.wheelbase = 0.25, .max_steering = 0.5};

    std::vector<hil::Point> track(200);
    hil::fill_circle(track, hil::Point{.x = 0.0, .y = 0.0}, 2.0);

    std::ofstream track_file(argv[1]);
    if (!track_file) {
        std::cerr << "cannot open " << argv[1] << '\n';
        return 1;
    }
    track_file << "x,y\n";
    for (const hil::Point& p : track) {
        track_file << std::format("{},{}\n", p.x, p.y);
    }

    /* Start 0.5 m inside the track at point 0, pointing along it (counterclockwise at point 0 means straight up).
    The offset makes the controller visibly pull the car onto the track.
    */
    hil::VehicleState state{.x = track[0].x - 0.5, .y = track[0].y,
                            .heading = std::numbers::pi / 2.0, .speed = 1.0};

    std::cout << "t,x,y,heading,speed,steering\n";
    const long steps = std::lround(duration / dt);
    for (long i = 0; i <= steps; ++i) {
        const double t = static_cast<double>(i) * dt;
        const double steering = hil::pure_pursuit(state, track, params, lookahead);
        std::cout << std::format("{},{},{},{},{},{}\n", t, state.x, state.y, state.heading, state.speed, steering);

        state = hil::step_euler(state, hil::VehicleInput{.steering = steering}, params, dt);
    }
}
