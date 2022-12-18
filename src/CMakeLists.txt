target_sources(dndmanager
    PRIVATE
        launcher.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        launcher.cpp
)

configure_file(dndmanager_config.hpp.in dndmanager_config.hpp)

add_subdirectory(controllers)
add_subdirectory(models)
add_subdirectory(parsing)
