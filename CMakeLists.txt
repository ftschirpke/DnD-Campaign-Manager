cmake_minimum_required(VERSION 3.22)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

project(
    DnD-Campaign-Manager
    DESCRIPTION
    "A simple tool providing multiple features that are helpful for DMs managing their DnD5e-Campaigns."
    LANGUAGES CXX
    VERSION 0.1.2
)

set(DND_GUI_APP dndmanager)
set(DND_TESTS dndmanager_tests)
set(DND_CORE dndmanager_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(Compiler.cmake)

# special directories
set(DND_ASSET_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
set(DND_MOCK_DIRECTORY ${CMAKE_SOURCE_DIR}/mock)
set(DND_BENCH_DIRECTORY ${CMAKE_SOURCE_DIR}/bench)

# font
set(FONT_URL https://dotcolon.net/files/fonts/aileron_0102.zip)
file(DOWNLOAD ${FONT_URL} ${DND_ASSET_DIRECTORY}/aileron.zip SHOW_PROGRESS)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xfz aileron.zip
    WORKING_DIRECTORY ${DND_ASSET_DIRECTORY}
    RESULT_VARIABLE unzip_result
)

# Dependency management
include(Dependencies.cmake)

# Targets
include(DndCore.cmake)
include(DndGui.cmake)
include(DndTests.cmake)

# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
