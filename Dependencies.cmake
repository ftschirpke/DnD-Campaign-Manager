
include(FetchContent)

# ------------------------------------------------------------------------------
# CORE LIBRARIES

# nlohmann/json - JSON for Modern C++
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d # Release Aug 12, 2022
)

# fmt - string formatting
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG f5e54359df4c26b6230fc61d38aa294581393084 # Release 10.1.1
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
# CLI LIBRARIES

# cxxopts - command line parsing
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG eb787304d67ec22f7c3a184ee8b4c481d04357fd # Release v3.1.1
)

FetchContent_MakeAvailable(cxxopts)

# ------------------------------------------------------------------------------
# GUI LIBRARIES

# GLFW - OpenGL Windowing Library
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e # Release 3.3.8
)

FetchContent_MakeAvailable(glfw)

# OpenGL
find_package(OpenGL REQUIRED)

# Dear ImGui - Graphical User Interface
FetchContent_Populate(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG 64b1e448d20c9be9275af731c34b4c7bf14a8e95
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
    GIT_TAG 29f2e22d8fe033d7b5e78621ee9f3f0a753d7c47
    SOURCE_DIR imgui/imgui_filebrowser
)

# ------------------------------------------------------------------------------
# TESTING LIBRARIES

# Catch2 - testing framework
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG 3f0283de7a9c43200033da996ff9093be3ac84dc
)

FetchContent_MakeAvailable(catch2)
