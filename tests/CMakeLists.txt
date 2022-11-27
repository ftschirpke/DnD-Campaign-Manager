set(TESTS_SOURCE_FILES
    launcher_test.cpp
)

foreach (TEST_SRC_FILE IN LISTS TESTS_SOURCE_FILES)
    string(REPLACE "_test" "" SRC_FILE ${TEST_SRC_FILE})
    LIST(APPEND SOURCE_FILES_FOR_TEST "${SRC_FILE}")
endforeach ()

foreach (SRC_FILE_FOR_TEST IN LISTS SOURCE_FILES_FOR_TEST)
    LIST(APPEND TESTS_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/${SRC_FILE_FOR_TEST})
endforeach ()

set(TESTS_PROJECT_NAME dndmanager_tests)

add_executable(${TESTS_PROJECT_NAME} ${TESTS_SOURCE_FILES})
target_link_libraries(${TESTS_PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

target_include_directories(
    ${TESTS_PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}/src
)

target_include_directories(
    ${TESTS_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/lib
)

include(CTest)
include(Catch)
catch_discover_tests(${TESTS_PROJECT_NAME}) # include catch2 tests
