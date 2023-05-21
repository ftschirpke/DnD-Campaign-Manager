target_sources(${DND_APP_TARGET}
    PRIVATE
    character.cpp
    character_state.cpp
    spell.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
    character.cpp
    character_state.cpp
    spell.cpp
)

add_subdirectory(effect_holder)
