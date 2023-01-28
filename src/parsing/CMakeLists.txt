target_sources(dndmanager
    PRIVATE
        content_file_parser.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        content_file_parser.cpp
)

add_subdirectory(controllers)
add_subdirectory(models)
