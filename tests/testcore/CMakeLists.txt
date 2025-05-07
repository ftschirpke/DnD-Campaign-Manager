target_sources(${DND_TESTS}
    PRIVATE
    minimal_testing_content.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(errors)
add_subdirectory(parsing)
add_subdirectory(searching)
add_subdirectory(text)
add_subdirectory(utils)
add_subdirectory(validation)
