target_sources(${DND_TESTS}
    PRIVATE
    validation_data_mock.cpp
    validation_data_test.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(character_class)
add_subdirectory(character_race)
add_subdirectory(character_subclass)
add_subdirectory(character_subrace)
add_subdirectory(effect_holder)
add_subdirectory(feature)
add_subdirectory(item)
add_subdirectory(spell)
