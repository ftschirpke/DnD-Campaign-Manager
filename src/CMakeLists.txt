configure_file(dnd_config.hpp.in dnd_config.hpp)

add_subdirectory(cli)
add_subdirectory(core)
add_subdirectory(gui)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(runtime_measurement)
endif()
