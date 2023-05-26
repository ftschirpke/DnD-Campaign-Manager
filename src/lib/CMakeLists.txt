add_library(imgui STATIC)
add_subdirectory(imgui)
target_link_libraries(${DND_GUI_APP} PRIVATE imgui)
