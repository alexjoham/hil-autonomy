#include "hil/angle.hpp"
#include <cmath>
#include <numbers>

using namespace std::numbers;

double hil::wrap_angle(double rad) noexcept {
    const double res = std::remainder(rad, 2 * pi);
    return res >= pi ? -pi : res;
}
