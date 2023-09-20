# Unit Tests
add_executable(${DND_TESTS})
set_target_properties(${DND_TESTS} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_TESTS}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    SYSTEM PRIVATE
    ${PROJECT_BINARY_DIR}/src
)
target_link_libraries(${DND_TESTS}
    PUBLIC
    ${DND_CORE} 
    PRIVATE
    Catch2::Catch2WithMain
)

set_compiler_flags(${DND_TESTS})

include(CTest)
enable_testing()

include(${catch2_SOURCE_DIR}/extras/Catch.cmake)

catch_discover_tests(${DND_TESTS}) # include catch2 tests
