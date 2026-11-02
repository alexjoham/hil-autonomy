# Teensy 4.1 toolchain

How the firmware for the Teensy 4.1 is built, without PlatformIO or the Arduino IDE.

## Setup

1. Download the Arm GNU Toolchain 11.3.rel1 for `arm-none-eabi`. This is the
   compiler version PJRC uses for the Teensy core.

   ```
   mkdir -p ~/toolchains && cd ~/toolchains
   curl -LO https://developer.arm.com/-/media/Files/downloads/gnu/11.3.rel1/binrel/arm-gnu-toolchain-11.3.rel1-darwin-x86_64-arm-none-eabi.tar.xz
   tar xf arm-gnu-toolchain-11.3.rel1-darwin-x86_64-arm-none-eabi.tar.xz
   ```

   On macOS, download with `curl`, not the browser. Browser downloads are
   quarantined and macOS refuses to run the binaries. On Apple silicon, this
   release runs through Rosetta 2.

2. Point `ARM_GCC_DIR` at the extracted folder, for example in `~/.zshrc`:

   ```
   export ARM_GCC_DIR=~/toolchains/arm-gnu-toolchain-11.3.rel1-darwin-x86_64-arm-none-eabi
   ```

   The toolchain is deliberately not added to `PATH`, so every build uses
   exactly this compiler.

## Build

```
cmake --workflow --preset teensy
```

Output goes to `build/teensy/`. Tests are not built for the Teensy.

## How it works

`cmake/teensy41.cmake` is the toolchain file:

- `CMAKE_SYSTEM_NAME Generic` means "no operating system". It also makes
  `CMAKE_CROSSCOMPILING` true, which is how the build skips tests and
  GoogleTest.
- The compiler path comes from the environment variable `ARM_GCC_DIR`.
  CMake reads the toolchain file several times, and environment variables
  are visible every time, cache variables are not.
- `CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY`: CMake tests the compiler by
  building a small program, but linking a Teensy program needs a linker
  script. Testing with a static library avoids the link step.

## Flags

Taken from a verbose PlatformIO build of the Teensy 4.1 (`pio run -v`).

| Flags | Purpose | Where |
|---|---|---|
| `-mthumb -mcpu=cortex-m7` | instruction set and CPU | toolchain file, everything |
| `-mfloat-abi=hard -mfpu=fpv5-d16` | hardware FPU, floats passed in FPU registers | toolchain file, everything including the link |
| `-ffunction-sections -fdata-sections` | one section per function and variable, so the linker can drop unused code | toolchain file |
| `-fno-exceptions -fno-rtti -fno-threadsafe-statics` | no exception tables, no RTTI, no locks for function-local statics | toolchain file, C++ only |
| `-Wall -Wextra ... -Werror` | project warnings | `hil_warnings` target, own code only |

The float ABI flags must be identical for every object in the link. Code
compiled for hard-float expects `float` arguments in FPU registers, code
compiled for soft-float in integer registers, and the linker refuses to mix
them.

## Checking the output

Which C library variant the flags select:

```
$ARM_GCC_DIR/bin/arm-none-eabi-gcc -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -print-multi-directory
```

Expected: `thumb/v7e-m+dp/hard`.

Disassembly of a built library, to confirm it contains Cortex-M7 code:

```
$ARM_GCC_DIR/bin/arm-none-eabi-objdump -d build/teensy/core/libhil_core.a
```
