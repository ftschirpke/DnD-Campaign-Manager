target_sources(${DND_GUI_APP}
    PRIVATE
    gui_app.cpp
    gui_launcher.cpp
)

add_subdirectory(content_visitors)
add_subdirectory(string_formatting)
