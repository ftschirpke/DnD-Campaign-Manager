target_sources(${DND_TESTS}
    PRIVATE
    character_file_parser_test.cpp
    spells_file_parser_test.cpp
)

add_subdirectory(effect_holder)
