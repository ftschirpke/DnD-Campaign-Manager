cmake_minimum_required(VERSION 3.20)

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

# CPM
    set(CPM_DOWNLOAD_VERSION 0.34.0)

    if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    endif()
    
    if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
    https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
    ${CPM_DOWNLOAD_LOCATION}
    )
    endif()
    
    include(${CPM_DOWNLOAD_LOCATION})
# end CPM

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.2
    OPTIONS
        "JSON_BuildTests OFF"
)

CPMAddPackage(
    NAME catch2
    VERSION 3.2.0
    GITHUB_REPOSITORY catchorg/Catch2
)

CPMAddPackage(
    NAME cxxopts
    GITHUB_REPOSITORY jarro2783/cxxopts
    VERSION 3.0.0
    OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set_property(GLOBAL PROPERTY CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-pedantic -Wall -Wextra -Wpedantic -Werror -Winline -Wdisabled-optimization -Wcast-qual
    -Wlogical-op -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion
    -Wstrict-null-sentinel -Wstrict-overflow=5 -Wzero-as-null-pointer-constant -Wuseless-cast)
endif()

add_executable(${DND_APP_TARGET} src/main.cpp)
set_target_properties(${DND_APP_TARGET} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_APP_TARGET}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(${DND_APP_TARGET} PUBLIC nlohmann_json::nlohmann_json cxxopts::cxxopts)

add_executable(${DND_TEST_TARGET})
set_target_properties(${DND_TEST_TARGET} PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${DND_TEST_TARGET}
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(${DND_TEST_TARGET} PRIVATE nlohmann_json::nlohmann_json cxxopts::cxxopts Catch2::Catch2WithMain)

include(CTest)
enable_testing()

include(Catch.cmake)

catch_discover_tests(${DND_TEST_TARGET}) # include catch2 tests

add_subdirectory(src)

add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
