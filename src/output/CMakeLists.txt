target_sources(${DND_APP_TARGET}
    PRIVATE
    command_line_output.cpp
)

target_sources(${DND_TEST_TARGET}
    PRIVATE
    command_line_output.cpp
)

add_subdirectory(cards)
add_subdirectory(latex_builder)
