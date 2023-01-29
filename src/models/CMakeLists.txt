target_sources(dndmanager
    PRIVATE
    character.cpp
    character_state.cpp
)

target_sources(dndmanager_tests
    PRIVATE
    character.cpp
    character_state.cpp
)

add_subdirectory(effect_holder)
