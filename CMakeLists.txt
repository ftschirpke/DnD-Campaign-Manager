cmake_minimum_required(VERSION 3.22)

project(DnD-Campaign-Manager
    DESCRIPTION "A simple tool providing multiple features that are helpful for DMs managing their DnD5e-Campaigns."
    LANGUAGES CXX
    VERSION 0.1.2
)

set(DND_CMD_APP dndmanager_cmd)
set(DND_GUI_APP dndmanager_gui)
set(DND_TESTS dndmanager_tests)
set(DND_CORE dndmanager_core)

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

FetchContent_MakeAvailable(nlohmann_json catch2 cxxopts fmt)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# ------------------------------------------------------------------------------
# DND core library
add_library(${DND_CORE} STATIC)
set_target_properties(${DND_CORE} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_CORE}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
)
target_link_libraries(${DND_CORE}
    PUBLIC
    nlohmann_json::nlohmann_json
    fmt::fmt
)

if(MSVC)
    target_compile_options(${DND_CORE} PRIVATE /W4 /WX)
    target_compile_definitions(${DND_CORE} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${DND_CORE} PRIVATE
        -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization -Wcast-qual -Wold-style-cast
        -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wstrict-overflow=5 -Wzero-as-null-pointer-constant
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${DND_CORE} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast)
    endif()
endif()

# ------------------------------------------------------------------------------
# Command Line Application
add_executable(${DND_CMD_APP} src/cmd_main.cpp)
set_target_properties(${DND_CMD_APP} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_CMD_APP}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
)
target_link_libraries(${DND_CMD_APP}
    PUBLIC
    ${DND_CORE}
    PRIVATE
    cxxopts::cxxopts
)

if(MSVC)
    target_compile_options(${DND_CMD_APP} PRIVATE /W4 /WX)
    target_compile_definitions(${DND_CMD_APP} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${DND_CMD_APP} PRIVATE
        -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization -Wcast-qual -Wold-style-cast
        -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wstrict-overflow=5 -Wzero-as-null-pointer-constant
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${DND_CMD_APP} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast)
    endif()
endif()

# ------------------------------------------------------------------------------
# Graphical User Interface Application
add_executable(${DND_GUI_APP} src/gui_main.cpp)
set_target_properties(${DND_GUI_APP} PROPERTIES EXPORT_COMPILE_COMMANDS ON)
set_target_properties(${DND_GUI_APP} PROPERTIES WIN32_EXECUTABLE TRUE)

target_include_directories(${DND_GUI_APP}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
)
target_link_libraries(${DND_GUI_APP}
    PUBLIC
    ${DND_CORE}
)

if (MSVC)
    # Tell MSVC to use main instead of WinMain
    set_target_properties(${DND_GUI_APP} PROPERTIES
                          LINK_FLAGS "/ENTRY:mainCRTStartup")
elseif (CMAKE_C_SIMULATE_ID STREQUAL "MSVC")
    # Tell Clang using MS CRT to use main instead of WinMain
    set_target_properties(${DND_GUI_APP} PROPERTIES
                          LINK_FLAGS "-Wl,/entry:mainCRTStartup")
endif()

if(MSVC)
    target_compile_options(${DND_GUI_APP} PRIVATE /W4 /WX)
    target_compile_definitions(${DND_GUI_APP} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${DND_GUI_APP} PRIVATE
        -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization -Wcast-qual -Wold-style-cast
        -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wstrict-overflow=5 -Wzero-as-null-pointer-constant
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${DND_GUI_APP} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast)
    endif()
endif()

# ------------------------------------------------------------------------------
# Unit Tests
add_executable(${DND_TESTS})
set_target_properties(${DND_TESTS} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_TESTS}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/core
)
target_link_libraries(${DND_TESTS}
    PRIVATE
    ${DND_CORE} nlohmann_json::nlohmann_json cxxopts::cxxopts fmt::fmt Catch2::Catch2WithMain
)

if(MSVC)
    target_compile_options(${DND_TESTS} PRIVATE /W4 /WX)
    target_compile_definitions(${DND_TESTS} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${DND_TESTS} PRIVATE
        -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization -Wcast-qual -Wold-style-cast
        -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wstrict-overflow=5 -Wzero-as-null-pointer-constant
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${DND_TESTS} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast)
    endif()
endif()

include(CTest)
enable_testing()

include(Catch.cmake)

catch_discover_tests(${DND_TESTS}) # include catch2 tests

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# ------------------------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(tests)
