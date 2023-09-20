configure_file(dnd_config.hpp.in dnd_config.hpp)

add_subdirectory(cli_app)
add_subdirectory(core)
add_subdirectory(gui_app)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(runtime_measurement)
endif()
