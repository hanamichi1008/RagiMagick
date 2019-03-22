cmake_minimum_required(VERSION 3.14)

project(ragii CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
    # add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    # add_compile_options(-Wall -Wextra -pedantic -Werror -Wunused-function)
    add_compile_options(-Wall -Wextra -pedantic)
    add_compile_options(-mfma4 -mxop -msse2 -msse3 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mxsave)
endif()


file(GLOB RAGII_TEXT_HEADERS include/text/*.h)
file(GLOB RAGII_TYPE_HEADERS include/type/*.h)
file(GLOB RAGII_SOURCES src/*.cpp)
source_group("Headers" FILES ${RAGII_TEXT_HEADERS})
source_group("Headers" FILES ${RAGII_TYPE_HEADERS})
source_group("Sources" FILES ${RAGII_SOURCES})

add_library(ragii STATIC
    ${RAGII_TEXT_HEADERS}
    ${RAGII_TYPE_HEADERS}
    ${RAGII_SOURCES}
)

target_include_directories(ragii PUBLIC "include")
