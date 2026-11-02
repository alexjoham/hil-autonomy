add_executable(firmware src/main.cpp)
set_target_properties(firmware PROPERTIES SUFFIX ".elf")
target_link_libraries(firmware PRIVATE teensy_core hil_warnings)
target_link_options(firmware PRIVATE -Wl,--gc-sections,--relax)

add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:firmware> $<TARGET_FILE_DIR:firmware>/firmware.hex
    COMMAND ${ARM_SIZE} $<TARGET_FILE:firmware>)
