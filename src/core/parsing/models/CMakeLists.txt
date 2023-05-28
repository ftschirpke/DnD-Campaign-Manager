target_sources(${DND_CORE}
    PRIVATE
    character_file_parser.cpp
    character_class_file_parser.cpp
    character_race_file_parser.cpp
    character_subclass_file_parser.cpp
    character_subrace_file_parser.cpp
    items_file_parser.cpp
    spells_file_parser.cpp
)

add_subdirectory(effect_holder)
add_subdirectory(spellcasting)
