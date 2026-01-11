set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
set_property(GLOBAL PROPERTY CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
set_property(GLOBAL PROPERTY CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# function that sets compiler flags for a target
function(set_compiler_flags TARGET)
    if(MSVC)
        target_compile_options(${TARGET} PRIVATE /W4 /WX)
        target_compile_definitions(${TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
    else()
        target_compile_options(${TARGET} PRIVATE
            -pedantic -Wall -Wextra -Wpedantic -Werror -Wdisabled-optimization -Wcast-qual -Wold-style-cast
            -Woverloaded-virtual -Wredundant-decls -Wstrict-overflow=2
            -fsanitize=address
        )
        target_link_options(${TARGET} PRIVATE
            -fsanitize=address
        )
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(${TARGET} PRIVATE -Wlogical-op -Wnoexcept -Wstrict-null-sentinel -Wuseless-cast)
        endif()
    endif()
endfunction()
