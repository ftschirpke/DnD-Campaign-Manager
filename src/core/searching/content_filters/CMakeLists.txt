target_sources(${DND_CORE}
    PRIVATE
    content_piece_filter.cpp
    bool_filter.cpp
    string_filter.cpp
)

add_subdirectory(character)
add_subdirectory(class)
add_subdirectory(subclass)
add_subdirectory(species)
add_subdirectory(subspecies)
add_subdirectory(effects_provider)
add_subdirectory(item)
add_subdirectory(spell)
