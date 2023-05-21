target_sources(${DND_APP_TARGET}
    PRIVATE
        content_parser.cpp
        effect_holder_groups_file_parser.cpp
        string_groups_file_parser.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
        content_parser.cpp
        effect_holder_groups_file_parser.cpp
        string_groups_file_parser.cpp
)
