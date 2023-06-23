target_sources(${DND_CORE}
    PRIVATE
    ability_scores_data.cpp
    character_data.cpp
    character_basis_data.cpp
    progression_data.cpp
)

add_subdirectory(decision)
