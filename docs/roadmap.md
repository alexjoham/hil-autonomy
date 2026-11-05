# Roadmap

## Idea

A simulated car drives around a track on the laptop.
Its autonomy stack (estimator and controller) runs on a Teensy 4.1, connected over USB serial: hardware-in-the-loop.
Later, one component is replaced by a small neural network running on the Teensy and compared against the classical version.

Focus: embedded C++ below the Arduino layer, real-time loops, autonomous driving basics, and ML inference on a microcontroller.

## Architecture

| Part | Contents | Builds for |
|---|---|---|
| `core/` | vehicle model, estimator, controller | host and Teensy |
| `link/` | serial protocol | host and Teensy |
| `sim/` | simulator, track, sensor noise, CSV output | host |
| `firmware/` | Teensy main loop, thin | Teensy |

`core` depends on nothing. No heap, no exceptions, no I/O in `core` and `link`.

## Milestones

| | Milestone | Status |
|---|---|---|
| M1 | Vehicle model, simulator loop, track, pure pursuit controller, path plot | in progress |
| M2 | Noisy sensors and an EKF | planned |
| M3 | Serial protocol between laptop and Teensy | planned |
| M4 | Autonomy stack runs on the Teensy in the loop, timer-driven, timing measured | planned |
| M5 | Below the Arduino layer: own timer setup, lock-free ring buffer, no heap, memory placement | planned |
| M6 | Small neural network library, int8 inference on the Teensy, compared with the classical controller | planned |

## Current state

Done: build setup (host and Teensy, CI), `wrap_angle`, kinematic bicycle model with Euler integration and tests.

Next: simulator loop writing CSV, plus a Python plot of the driven path.

## Decisions

- Own CMake toolchain instead of PlatformIO, to understand the target build.
- Radians and SI units everywhere inside `core`.
- Model as plain structs and pure functions, so it is easy to test and runs unchanged on the Teensy.
- Euler integration first, RK4 later.
