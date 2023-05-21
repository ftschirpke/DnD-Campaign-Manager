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

add_library(imgui STATIC)
target_include_directories(imgui PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(imgui PUBLIC ${OPENGL_LIBRARIES} glfw)

if(MSVC)
    target_compile_options(imgui PRIVATE /W4 /WX)
    target_compile_definitions(imgui PRIVATE -D_CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(imgui PRIVATE
        -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization
        -Woverloaded-virtual -Wredundant-decls
    )
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(imgui PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel)
    endif()
endif()

add_subdirectory(imgui)

target_link_libraries(${DND_GUI_APP} PRIVATE imgui)
target_include_directories(${DND_GUI_APP} SYSTEM PRIVATE imgui)
