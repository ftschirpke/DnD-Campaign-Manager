# TODO: better way of adding test files
set(TEST_FILES
    launcher_test.cpp
    parsing/content_parser_test.cpp
    parsing/models/character_file_parser_test.cpp
    parsing/models/feature_holder_file_parser_test.cpp
)

set(TEST_SOURCE_FILES
    PRIVATE
        ${TEST_FILES}
)

# TODO: better way of adding the needed source files
foreach (TEST_SRC_FILE IN LISTS TEST_FILES)
    string(REPLACE "_test" "" SRC_FILE ${TEST_SRC_FILE})
    LIST(APPEND SOURCE_FILES_FOR_TEST "${SRC_FILE}")
endforeach ()

foreach (SRC_FILE_FOR_TEST IN LISTS SOURCE_FILES_FOR_TEST)
    LIST(APPEND TEST_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/${SRC_FILE_FOR_TEST})
endforeach ()

target_sources(dndmanager_tests
    PRIVATE
        ${TEST_SOURCE_FILES}
)

