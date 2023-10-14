target_sources(${DND_CORE}
    PRIVATE
    source_info.cpp
)

add_subdirectory(character)
add_subdirectory(character_class)
add_subdirectory(character_race)
add_subdirectory(character_subclass)
add_subdirectory(character_subrace)
add_subdirectory(effects)
add_subdirectory(effects_provider)
add_subdirectory(item)
add_subdirectory(spell)
add_subdirectory(spellcasting)
