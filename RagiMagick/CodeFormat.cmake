
function(format_code target_name)

    find_program(CLANG_FORMAT_EXE clang-format)
    if(NOT CLANG_FORMAT_EXE)
        return()
    endif()

    file(GLOB_RECURSE MY_ALL_SOURCES *.h *.c *.cpp)
    add_custom_target(
        ${target_name}
        COMMAND "${CLANG_FORMAT_EXE}" -style=file -i ${MY_ALL_SOURCES}
        COMMENT "formatting..."
    )

endfunction()
