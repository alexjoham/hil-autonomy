#include "hil/pure_pursuit.hpp"
#include "hil/track.hpp"
#include "hil/angle.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>


double hil::pure_pursuit(const VehicleState& state, std::span<const Point> track, const VehicleParams& params, double lookahead) {
    assert(!track.empty());
    assert(lookahead > 0);
    std::size_t start = nearest_index(track, {state.x, state.y});
    const Point car{.x = state.x, .y = state.y};
    Point target = track[start];
    for (size_t k = 0; k < track.size(); ++k) {
        const std::size_t index = (start + k) % track.size();
        const double dist = distance(car, track[index]);
        target = track[index];
        if (dist > lookahead) {
            break;
        }
    }
    const double alpha = wrap_angle(std::atan2(target.y - state.y, target.x - state.x) - state.heading);
    const double steering = std::clamp(std::atan(2.0 * params.wheelbase * std::sin(alpha) / lookahead), -params.max_steering, params.max_steering);

    return steering;
}
