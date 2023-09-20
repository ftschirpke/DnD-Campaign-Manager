# Graphical User Interface Application
add_executable(${DND_GUI_APP} src/gui_main.cpp)
set_target_properties(${DND_GUI_APP} PROPERTIES EXPORT_COMPILE_COMMANDS ON)
set_target_properties(${DND_GUI_APP} PROPERTIES WIN32_EXECUTABLE TRUE)

target_include_directories(${DND_GUI_APP}
    SYSTEM PRIVATE
    ${PROJECT_BINARY_DIR}/src
    ${PROJECT_BINARY_DIR}/imgui
    ${PROJECT_BINARY_DIR}/imgui/imgui # TODO: find a better way to do this (imgui.h)
)
target_link_libraries(${DND_GUI_APP}
    PUBLIC
    ${DND_CORE}
    PRIVATE
    imgui
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

set_compiler_flags(${DND_GUI_APP})
