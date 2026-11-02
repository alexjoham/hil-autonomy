# hil-autonomy

Autonomous vehicle stack (estimator, controller, learned component) running on a Teensy 4.1, hardware-in-the-loop with a C++ simulator.

This repository is still work in progress!

## Building

Host build with tests (GCC or Clang):

```
cmake --workflow --preset debug
```

Teensy 4.1 build: see [docs/teensy-toolchain.md](docs/teensy-toolchain.md).
