target_sources(${DND_CORE}
    PRIVATE
    content.cpp
    groups.cpp
    session.cpp
)

add_subdirectory(basic_mechanics)
add_subdirectory(visitors)
add_subdirectory(errors)
add_subdirectory(exceptions)
add_subdirectory(models)
add_subdirectory(output)
add_subdirectory(parsing)
add_subdirectory(searching)
add_subdirectory(text)
add_subdirectory(utils)
add_subdirectory(validation)
