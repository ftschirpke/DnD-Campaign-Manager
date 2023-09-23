cmake_minimum_required(VERSION 3.22)

project(DnD-Campaign-Manager
    DESCRIPTION "A simple tool providing multiple features that are helpful for DMs managing their DnD5e-Campaigns."
    LANGUAGES CXX
    VERSION 0.1.2
)

set(DND_CLI_APP dndmanager_cli)
set(DND_GUI_APP dndmanager_gui)
set(DND_TESTS dndmanager_tests)
set(DND_CORE dndmanager_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(Compiler.cmake)

# special directories
set(DND_ASSET_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
set(DND_MOCK_DIRECTORY ${CMAKE_SOURCE_DIR}/mock)
set(DND_DEBUG_DIRECTORY ${CMAKE_SOURCE_DIR}/debug)

# Dependency management
include(Dependencies.cmake)

# Targets
include(DndCore.cmake)
include(DndCli.cmake)
include(DndGui.cmake)
include(DndTests.cmake)

# Subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
