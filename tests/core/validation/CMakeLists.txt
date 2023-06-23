target_sources(${DND_TESTS}
    PRIVATE
    validation_data_mock.cpp
    validation_data_test.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(item)
