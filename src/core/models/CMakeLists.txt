target_sources(${DND_CORE}
    PRIVATE
    content_piece.cpp
)

add_subdirectory(character)
add_subdirectory(character_class)
add_subdirectory(character_race)
add_subdirectory(character_subclass)
add_subdirectory(character_subrace)
add_subdirectory(effect_holder)
add_subdirectory(feature)
add_subdirectory(feature_holder)
add_subdirectory(item)
add_subdirectory(spell)
