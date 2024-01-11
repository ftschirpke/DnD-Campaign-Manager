target_sources(${DND_CORE}
    PRIVATE
    ability_scores_validation.cpp
    character_validation.cpp
    feature_providers_validation.cpp
    progression_validation.cpp
)

add_subdirectory(decision)
