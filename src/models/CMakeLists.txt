target_sources(dndmanager
    PRIVATE
    character.cpp
    character_state.cpp
    spell.cpp
)

target_sources(dndmanager_tests
    PRIVATE
    character.cpp
    character_state.cpp
    spell.cpp
)

add_subdirectory(effect_holder)
