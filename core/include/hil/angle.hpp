#pragma once

namespace hil {

    /**
     * Wraps an angle in radians into [-pi, pi). Returns NaN for non-finite input.
     */
    [[nodiscard]] double wrap_angle(double rad) noexcept;
}
