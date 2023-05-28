set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 7482de6071d21db77a7236155da44c172a7f6c9e
)

FetchContent_MakeAvailable(glfw)

find_package(OpenGL REQUIRED)

target_sources(imgui
    PRIVATE
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
    imgui_stdlib.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui.cpp
)

target_include_directories(imgui PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES} glfw)
