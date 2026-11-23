#pragma once
#include <cmath>

namespace hil
{
    struct Point {
        double x{0.0};
        double y{0.0};

        bool operator==(const Point&) const = default;
    };

    double distance(Point a, Point b);
} // namespace hil
