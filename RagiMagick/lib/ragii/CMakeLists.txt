cmake_minimum_required(VERSION 3.14)

project(ragii CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    # add_compile_options(-Wall -Wextra -pedantic -Werror -Wunused-function)
    add_compile_options(-Wall -Wextra -pedantic)
endif()


file(GLOB RAGII_HEADERS include/*.h)
file(GLOB RAGII_SOURCES src/*.cpp)
file(GLOB RAGII_HARDWARE_HEADERS include/hardware/*.h)
file(GLOB RAGII_HARDWARE_SOURCES src/hardware/*.cpp)
file(GLOB RAGII_TEXT_HEADERS include/text/*.h)
file(GLOB RAGII_TYPE_HEADERS include/type/*.h)

source_group("Headers" FILES ${RAGII_HEADERS})
source_group("Sources" FILES ${RAGII_SOURCES})
source_group("Headers\\hardware" FILES ${RAGII_HARDWARE_HEADERS})
source_group("Sources\\hardware" FILES ${RAGII_HARDWARE_SOURCES})
source_group("Headers\\text" FILES ${RAGII_TEXT_HEADERS})
source_group("Headers\\type" FILES ${RAGII_TYPE_HEADERS})

add_library(ragii STATIC
    ${RAGII_HEADERS}
    ${RAGII_SOURCES}
    ${RAGII_HARDWARE_HEADERS}
    ${RAGII_HARDWARE_SOURCES}
    ${RAGII_TEXT_HEADERS}
    ${RAGII_TYPE_HEADERS}
)

target_include_directories(ragii PUBLIC "include")
