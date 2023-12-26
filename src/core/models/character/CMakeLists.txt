target_sources(${DND_CORE}
    PRIVATE
    ability_scores.cpp
    character.cpp
    feature_providers.cpp
    progression.cpp
)

add_subdirectory(decision)
