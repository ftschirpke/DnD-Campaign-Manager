target_sources(${DND_APP_TARGET}
    PRIVATE
        content_file_parser.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
        content_file_parser.cpp
)

add_subdirectory(controllers)
add_subdirectory(models)
