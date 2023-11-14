target_sources(${DND_CORE}
    PRIVATE
    content_parsing.cpp
    file_parser.cpp
    parser.cpp
)

add_subdirectory(character)
add_subdirectory(character_class)
add_subdirectory(character_subclass)
add_subdirectory(character_race)
add_subdirectory(character_subrace)
add_subdirectory(effects)
add_subdirectory(effects_provider)
add_subdirectory(groups)
add_subdirectory(item)
add_subdirectory(spell)
