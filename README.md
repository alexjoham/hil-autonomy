# hil-autonomy

Autonomous vehicle stack (estimator, controller, learned component) running on a Teensy 4.1, hardware-in-the-loop with a C++ simulator.

This repository is still work in progress!

## Building

Host build with tests (GCC or Clang):

```
cmake --workflow --preset debug
```

## Simulation

To run the simulation call:
```
./build/debug/sim/hil_sim > run.csv
```

This then creates a file `run.csv`. To plot the simulation run:

```
python3 tools/plot_run.py run.csv run.png
```

This then creates the file `run.png` that visualizes the run in a png.

Teensy 4.1 build: see [docs/teensy-toolchain.md](docs/teensy-toolchain.md).
