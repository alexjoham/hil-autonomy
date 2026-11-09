add_executable(hil_sim src/main.cpp)

target_link_libraries(hil_sim PRIVATE hil::core hil_warnings)
