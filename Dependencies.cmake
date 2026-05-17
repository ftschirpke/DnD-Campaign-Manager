
include(FetchContent)

# ------------------------------------------------------------------------------
# CORE LIBRARIES

# nlohmann/json - JSON for Modern C++
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG 55f93686c01528224f448c19128836e7df245f72 # Release Apr 11, 2025
)

# fmt - string formatting
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 407c905e45ad75fc29bf0f9bb7c5c2fd3475976f # Release 12.1.0
)

FetchContent_MakeAvailable(nlohmann_json fmt)

# ------------------------------------------------------------------------------
# GUI LIBRARIES

# GLFW - OpenGL Windowing Library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # Release 3.4
)

FetchContent_MakeAvailable(glfw)

# OpenGL
find_package(OpenGL REQUIRED)

# Dear ImGui - Graphical User Interface
FetchContent_Populate(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG b61e56346a92cfcaf1f43a545ca37b0b32239654 # Release 1.92.8-docking
    SOURCE_DIR imgui/imgui
)

add_library(imgui STATIC)
target_sources(imgui
    PRIVATE
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)
target_include_directories(imgui
    PUBLIC
    ${OPENGL_INCLUDE_DIRS}
    PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_BINARY_DIR}
    ${imgui_SOURCE_DIR}/imgui
    ${imgui_BINARY_DIR}/imgui
)
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES} glfw)

# ImGui-FileBrowser - File Browser for Dear ImGui
FetchContent_Populate(
    imgui-filebrowser
    GIT_REPOSITORY https://github.com/AirGuanZ/imgui-filebrowser
    GIT_TAG 47a1884524d6a3df5d99f70ecc82e1ed0ccd26cf # Commit Sep 24, 2025
    SOURCE_DIR imgui/imgui_filebrowser
)

# ------------------------------------------------------------------------------
# TESTING LIBRARIES

# Catch2 - testing framework
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 6ee0826dcae55ed1e06b2c5701981221e979e1e6 # Release 3.15.0
)

FetchContent_MakeAvailable(catch2)
