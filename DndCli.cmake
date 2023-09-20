# Command Line Application
add_executable(${DND_CLI_APP} src/cli_main.cpp)
set_target_properties(${DND_CLI_APP} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_CLI_APP}
    SYSTEM PRIVATE
    ${PROJECT_BINARY_DIR}/src
)
target_link_libraries(${DND_CLI_APP}
    PUBLIC
    ${DND_CORE}
    PRIVATE
    cxxopts::cxxopts
)

set_compiler_flags(${DND_CLI_APP})
