
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
    GIT_TAG 123913715afeb8a437e6388b4473fcc4753e1c9a # Release 11.1.4
)

# tl::expected - expected monad
FetchContent_Populate(
    tl_expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected
    GIT_TAG 292eff8bd8ee230a7df1d6a1c00c4ea0eb2f0362 # Release v1.1.0
    SOURCE_DIR tl_expected
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
    GIT_TAG 4806a1924ff6181180bf5e4b8b79ab4394118875 # Release 1.91.9b-docking
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
    GIT_TAG 347dda538f37fb71be3767e66930e0d7bc5d5f52 # Commit Feb 17, 2025
    SOURCE_DIR imgui/imgui_filebrowser
)

# ------------------------------------------------------------------------------
# TESTING LIBRARIES

# Catch2 - testing framework
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 2b60af89e23d28eefc081bc930831ee9d45ea58b # Release 3.8.1
)

FetchContent_MakeAvailable(catch2)
