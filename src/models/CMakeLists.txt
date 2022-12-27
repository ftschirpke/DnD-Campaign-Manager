target_sources(dndmanager
    PRIVATE
    character.cpp
    creature.cpp
    creature_state.cpp
)

target_sources(dndmanager_tests
    PRIVATE
    character.cpp
    creature.cpp
    creature_state.cpp
)

add_subdirectory(features)
