#include "hil/track.hpp"
#include "hil/geometry.hpp"
#include <cassert>
#include <cmath>
#include <numbers>

void hil::fill_circle(std::span<Point> out, Point center, double radius) {
    const std::size_t n = out.size();
    for (std::size_t i = 0; i < n; i++) {
        const double angle = 2.0 * std::numbers::pi * static_cast<double>(i) / static_cast<double>(n);
        out[i] = Point{.x = center.x + radius * std::cos(angle), .y = center.y + radius * std::sin(angle)};
    }
}

std::size_t hil::nearest_index(std::span<const Point> track, Point position) {
    assert(!track.empty());
    std::size_t cur_index = 0;
    double cur_distance = distance(position, track[0]);
    for (std::size_t i = 1; i < track.size(); i++) {
        const double new_distance = distance(position, track[i]);
        if (new_distance < cur_distance) {
            cur_distance = new_distance;
            cur_index = i;
        }
    }
    return cur_index;
}
