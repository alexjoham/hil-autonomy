add_executable(hil_core_tests angle_test.cpp vehicle_test.cpp)
target_link_libraries(hil_core_tests PRIVATE hil::core hil_warnings GTest::gtest_main)
gtest_discover_tests(hil_core_tests)
