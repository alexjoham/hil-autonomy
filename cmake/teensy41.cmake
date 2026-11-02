set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

if(NOT DEFINED ENV{ARM_GCC_DIR})
    message(FATAL_ERROR "ARM_GCC_DIR is not set. Point it at the root folder of the Arm GNU toolchain.")
endif()

set(CMAKE_C_COMPILER   "$ENV{ARM_GCC_DIR}/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "$ENV{ARM_GCC_DIR}/bin/arm-none-eabi-g++")
set(CMAKE_ASM_COMPILER "$ENV{ARM_GCC_DIR}/bin/arm-none-eabi-gcc")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TEENSY_MACHINE_FLAGS "-mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16")
set(CMAKE_C_FLAGS_INIT   "${TEENSY_MACHINE_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_INIT "${TEENSY_MACHINE_FLAGS} -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics")
set(CMAKE_ASM_FLAGS_INIT "${TEENSY_MACHINE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${TEENSY_MACHINE_FLAGS}")