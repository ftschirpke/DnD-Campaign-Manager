target_sources(${DND_TEST_TARGET}
    PRIVATE
    character_file_parser_test.cpp
    spells_file_parser_test.cpp
)

add_subdirectory(effect_holder)
