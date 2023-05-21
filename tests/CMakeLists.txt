# TODO: better way of adding test files
set(TEST_FILES
    launcher_test.cpp
    controllers/trie_test.cpp
    parsing/controllers/content_parser_test.cpp
    parsing/models/character_file_parser_test.cpp
    parsing/models/effect_holder/effect_holder_parser_test.cpp
    parsing/models/effect_holder/features_parser_test.cpp
    parsing/models/spells_file_parser_test.cpp
)

set(TEST_SOURCE_FILES
    PRIVATE
        ${TEST_FILES}
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
        ${TEST_SOURCE_FILES}
)

