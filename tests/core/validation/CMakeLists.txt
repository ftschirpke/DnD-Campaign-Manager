target_sources(${DND_TESTS}
    PRIVATE
    minimal_testing_content.cpp
    validation_data_mock.cpp
    validation_data_test.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(effect_holder)
add_subdirectory(feature)
add_subdirectory(item)
add_subdirectory(spell)
