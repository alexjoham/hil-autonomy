#pragma once

namespace hil {

    struct VehicleState {
        double x{0.0};
        double y{0.0};
        double heading{0.0};
        double speed{0.0};

        bool operator==(const VehicleState&) const = default;
    };

    struct VehicleInput {
        double steering{0.0};
        double acceleration{0.0};
    };

    struct VehicleParams {
        double wheelbase;
        double max_steering;
    };

    /**
     * Calculates the derivative of the new Vehicle state based on the following formulas:
     * x' = speed * cos(heading)
     * y' = speed * sin(heading)
     * heading' = (speed / wheelbase) * tan(steering)
     * speed' = acceleration
     * @param s VehicleState before
     * @param u VehicleInput
     * @param p VehicleParams, with wheelbase > 0 and max_steering >= 0
     * @returns the rates of change, stored in a VehicleState
     */
    VehicleState derivative(const VehicleState& s, const VehicleInput& u, const VehicleParams& p);

    /**
     * calculates the new value for a constant time dt
     * @param s VehicleState before
     * @param u VehicleInput
     * @param p VehicleParams, with wheelbase > 0 and max_steering >= 0
     * @returns the new state, stored in a VehicleState
     */
    VehicleState step_euler(const VehicleState& s, const VehicleInput& u, const VehicleParams& p, double dt);
}
