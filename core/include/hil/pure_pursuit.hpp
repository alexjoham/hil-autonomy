#pragma once
#include "hil/vehicle.hpp"
#include "hil/geometry.hpp"
#include <span>

namespace hil
{
    [[nodiscard]] double pure_pursuit(const VehicleState& state, std::span<const Point> track, const VehicleParams& params, double lookahead);
} // namespace hil
