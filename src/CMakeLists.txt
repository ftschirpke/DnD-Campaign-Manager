configure_file(dnd_config.hpp.in dnd_config.hpp)

add_subdirectory(cmd_app)
add_subdirectory(core)
add_subdirectory(gui_app)
add_subdirectory(lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(runtime_measurement)
endif()

target_include_directories(${DND_GUI_APP} SYSTEM PRIVATE lib)
