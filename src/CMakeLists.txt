target_sources(${DND_APP_TARGET}
    PRIVATE
    launcher.cpp
    imgui_launcher.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
    launcher.cpp
)

configure_file(dnd_config.hpp.in dnd_config.hpp)

add_subdirectory(controllers)
add_subdirectory(models)
add_subdirectory(output)
add_subdirectory(parsing)

add_subdirectory(lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(runtime_measurement)
endif()
