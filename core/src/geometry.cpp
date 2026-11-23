#include "hil/geometry.hpp"

double hil::distance(Point a, Point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}
