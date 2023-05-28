target_sources(${DND_CORE}
    PRIVATE
    command_line_output.cpp
)

add_subdirectory(cards)
add_subdirectory(latex_builder)
