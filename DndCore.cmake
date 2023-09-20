# DND core library
add_library(${DND_CORE} STATIC)
set_target_properties(${DND_CORE} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_CORE}
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    SYSTEM PRIVATE
    ${PROJECT_BINARY_DIR}/src
)
target_link_libraries(${DND_CORE}
    PUBLIC
    nlohmann_json::nlohmann_json
    fmt::fmt
)

set_compiler_flags(${DND_CORE})
