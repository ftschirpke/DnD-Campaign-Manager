target_sources(${DND_CLI_APP}
    PRIVATE
    cli_app.cpp
    cli_launcher.cpp
)

add_subdirectory(output)
add_subdirectory(visitors)
