target_sources(${DND_CORE}
    PRIVATE
    validation_data.cpp
    validation_subdata.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(character)
add_subdirectory(character_class)
add_subdirectory(character_subclass)
add_subdirectory(character_species)
add_subdirectory(character_subspecies)
add_subdirectory(effects)
add_subdirectory(effects_provider)
add_subdirectory(item)
add_subdirectory(spell)
add_subdirectory(spellcasting)
