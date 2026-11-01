add_library(hil_warnings INTERFACE)

target_compile_options(hil_warnings INTERFACE -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror)
