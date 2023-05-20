cmake_minimum_required(VERSION 3.22)

project(DnD-Campaign-Manager
    DESCRIPTION "A simple tool providing multiple features that are helpful for DMs managing their DnD5e-Campaigns."
    LANGUAGES CXX
    VERSION 0.1.2
)

set(DND_APP_TARGET dndmanager)
set(DND_TEST_TARGET dndmanager_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d
)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 3f0283de7a9c43200033da996ff9093be3ac84dc
)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG eb787304d67ec22f7c3a184ee8b4c481d04357fd
)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e
)

FetchContent_MakeAvailable(nlohmann_json catch2 cxxopts fmt glfw)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(
        -pedantic -Wall -Wextra -Wpedantic -Wdisabled-optimization
        -Woverloaded-virtual -Wredundant-decls
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options(-Wlogical-op -Wnoexcept -Wstrict-null-sentinel)
    endif()
endif()

# Main Application
add_executable(${DND_APP_TARGET} src/main.cpp)
set_target_properties(${DND_APP_TARGET} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_APP_TARGET}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(${DND_APP_TARGET} PUBLIC nlohmann_json::nlohmann_json cxxopts::cxxopts fmt::fmt glfw OpenGL::GL)

# Unit Tests
add_executable(${DND_TEST_TARGET})
set_target_properties(${DND_TEST_TARGET} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_TEST_TARGET}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(${DND_TEST_TARGET} PRIVATE nlohmann_json::nlohmann_json cxxopts::cxxopts fmt::fmt Catch2::Catch2WithMain)

include(CTest)
enable_testing()

include(Catch.cmake)

catch_discover_tests(${DND_TEST_TARGET}) # include catch2 tests

add_subdirectory(src)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
