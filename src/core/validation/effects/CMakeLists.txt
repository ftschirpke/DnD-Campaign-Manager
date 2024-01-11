target_sources(${DND_CORE}
    PRIVATE
    effects_validation.cpp
)

add_subdirectory(choice)
add_subdirectory(condition)
add_subdirectory(stat_change)
add_subdirectory(subholders)
