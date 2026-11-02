cmake_minimum_required(VERSION 3.25)
project(hil_autonomy LANGUAGES CXX)

include(cmake/warnings.cmake)

if(NOT CMAKE_CROSSCOMPILING)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.18.0
    )
    set(BUILD_GMOCK OFF)
    set(INSTALL_GTEST OFF)
    block()
        set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
        FetchContent_MakeAvailable(googletest)
    endblock()
    enable_testing()
    include(GoogleTest)
endif()

add_subdirectory(core)
