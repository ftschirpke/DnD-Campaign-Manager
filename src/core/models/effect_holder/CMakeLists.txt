target_sources(${DND_CORE}
    PRIVATE
    effect_holder.cpp
)

add_subdirectory(choice)
add_subdirectory(condition)
add_subdirectory(effect)
add_subdirectory(subholders)
