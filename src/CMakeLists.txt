set(SOURCE_FILES
    main.cpp
    launcher.cpp
)

set(EXEC_NAME dndmanager)

add_executable(${EXEC_NAME} ${SOURCE_FILES})

configure_file(dndmanager_config.hpp.in dndmanager_config.hpp)

target_include_directories(
    ${EXEC_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}/src
)

target_include_directories(
    ${EXEC_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
