cmake_minimum_required(VERSION 3.20)

project(DnDManager LANGUAGES CXX VERSION 0.1.2)

# CPM
    set(CPM_DOWNLOAD_VERSION 0.34.0)

    if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
    endif()
    
    if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(DOWNLOAD
    https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
    ${CPM_DOWNLOAD_LOCATION}
    )
    endif()
    
    include(${CPM_DOWNLOAD_LOCATION})
# end CPM

CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.2)

CPMAddPackage(
    NAME catch
    GITHUB_REPOSITORY catchorg/Catch2
    VERSION 3.2.0)

add_executable(dndmanager main.cpp)
set_target_properties(dndmanager PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(dndmanager
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(dndmanager PUBLIC nlohmann_json::nlohmann_json)

add_executable(dndmanager_tests)
set_target_properties(dndmanager_tests PROPERTIES EXPORT_COMPILE_COMMANDS ON)

target_include_directories(dndmanager_tests
    PUBLIC
    ${PROJECT_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/src
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib
)
target_link_libraries(dndmanager_tests PRIVATE nlohmann_json::nlohmann_json Catch2::Catch2WithMain)

include(CTest)
include(Catch.cmake)
catch_discover_tests(dndmanager_tests) # include catch2 tests


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

add_subdirectory(tests)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

