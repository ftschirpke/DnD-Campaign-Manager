target_sources(${DND_CORE}
    PRIVATE
    character.cpp
    character_class.cpp
    character_subclass.cpp
    character_race.cpp
    character_subrace.cpp
    character_state.cpp
    spell.cpp
)

add_subdirectory(effect_holder)
