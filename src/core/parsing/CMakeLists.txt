target_sources(${DND_CORE}
    PRIVATE
    content_parsing.cpp
    file_parser.cpp
    parser.cpp
    v2_file_parser.cpp
)

add_subdirectory(character)
add_subdirectory(class)
add_subdirectory(subclass)
add_subdirectory(species)
add_subdirectory(subspecies)
add_subdirectory(effects)
add_subdirectory(effects_provider)
add_subdirectory(groups)
add_subdirectory(item)
add_subdirectory(spell)
