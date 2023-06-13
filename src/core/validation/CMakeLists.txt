target_sources(${DND_CORE}
    PRIVATE
    validation_data.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(character_class)
add_subdirectory(character_subclass)
add_subdirectory(character_race)
add_subdirectory(character_subrace)
add_subdirectory(effect_holder)
add_subdirectory(feature)
add_subdirectory(spell)
