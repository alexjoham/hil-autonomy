set(TEENSY4_DIR ${CMAKE_CURRENT_SOURCE_DIR}/teensy-cores/teensy4)

file(GLOB TEENSY4_SOURCES CONFIGURE_DEPENDS
    ${TEENSY4_DIR}/*.c ${TEENSY4_DIR}/*.cpp ${TEENSY4_DIR}/*.S)

add_library(teensy_core STATIC ${TEENSY4_SOURCES})

target_include_directories(teensy_core SYSTEM PUBLIC ${TEENSY4_DIR})
target_compile_definitions(teensy_core PUBLIC
    __IMXRT1062__ ARDUINO_TEENSY41 F_CPU=600000000 USB_SERIAL
    ARDUINO=10805 TEENSYDUINO=159 CORE_TEENSY LAYOUT_US_ENGLISH)
set_target_properties(teensy_core PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_compile_options(teensy_core PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive -Wno-error=narrowing>)
