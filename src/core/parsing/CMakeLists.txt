target_sources(${DND_CORE}
    PRIVATE
    file_parser.cpp
    parser.cpp
)

add_subdirectory(item)
add_subdirectory(effect_holder)
add_subdirectory(feature)
add_subdirectory(spell)
