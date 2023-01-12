target_sources(dndmanager
    PRIVATE
        content_parser.cpp
        content_file_parser.cpp
        effects_holder_groups_file_parser.cpp
        string_groups_file_parser.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        content_parser.cpp
        content_file_parser.cpp
        effects_holder_groups_file_parser.cpp
        string_groups_file_parser.cpp
)

add_subdirectory(models)
