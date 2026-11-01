add_library(hil_core STATIC src/angle.cpp)
add_library(hil::core ALIAS hil_core)

target_include_directories(hil_core PUBLIC include)
target_compile_features(hil_core PUBLIC cxx_std_20)
target_link_libraries(hil_core PRIVATE hil_warnings)
