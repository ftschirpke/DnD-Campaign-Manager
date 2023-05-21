target_sources(${DND_APP_TARGET}
    PRIVATE
    character_file_parser.cpp
    character_class_file_parser.cpp
    character_race_file_parser.cpp
    character_subclass_file_parser.cpp
    character_subrace_file_parser.cpp
    items_file_parser.cpp
    spells_file_parser.cpp
)

target_sources(${DND_TEST_TARGET}
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
