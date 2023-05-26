if (TARGET ${DND_GUI_APP})
    add_subdirectory(imgui)

    target_link_libraries(${DND_GUI_APP} PRIVATE imgui)
    target_include_directories(${DND_GUI_APP} SYSTEM PRIVATE imgui)
endif() # TARGET ${DND_GUI_APP}
