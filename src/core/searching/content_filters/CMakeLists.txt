target_sources(${DND_CORE}
    PRIVATE
    content_piece_filter.cpp
    bool_filter.cpp
    string_filter.cpp
)

add_subdirectory(character)
add_subdirectory(character_class)
add_subdirectory(character_subclass)
add_subdirectory(spell)
