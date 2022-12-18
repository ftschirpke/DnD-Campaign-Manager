target_sources(dndmanager
    PRIVATE
        spell_parser.cpp
        character_race_parser.cpp
        feature_holder_parser.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        spell_parser.cpp
        character_race_parser.cpp
        feature_holder_parser.cpp
)

add_subdirectory(features)
