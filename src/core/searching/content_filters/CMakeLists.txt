target_sources(${DND_CORE}
    PRIVATE
    content_piece_filter.cpp
    bool_filter.cpp
    string_filter.cpp
)

add_subdirectory(character)
add_subdirectory(spell)
