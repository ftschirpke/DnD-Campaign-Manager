target_sources(dndmanager
    PRIVATE
        command_line_output.cpp
)

target_sources(dndmanager_tests
    PRIVATE
        command_line_output.cpp
)

add_subdirectory(cards)
add_subdirectory(latex_builder)
