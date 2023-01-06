target_sources(dndmanager
    PRIVATE
        launcher.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        launcher.cpp
)

configure_file(dnd_config.hpp.in dnd_config.hpp)

add_subdirectory(controllers)
add_subdirectory(models)
add_subdirectory(parsing)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(runtime_measurement)
endif()
