target_sources(dndmanager
    PRIVATE
        content_parser.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        content_parser.cpp
)

add_subdirectory(models)
