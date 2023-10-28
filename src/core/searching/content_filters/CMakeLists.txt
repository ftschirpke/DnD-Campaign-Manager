target_sources(${DND_CORE}
    PRIVATE
    content_piece_filter.cpp
    bool_filtering.cpp
    number_filtering.cpp
    string_filtering.cpp
)

add_subdirectory(spell)
