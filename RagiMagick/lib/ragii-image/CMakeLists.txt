cmake_minimum_required(VERSION 3.14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(ragii-image CXX)


# add_definitions(-DGLM_FORCE_INTRINSICS)
add_definitions(-DGLM_FORCE_PURE)

if (MSVC)
    # add_compile_options(/W4 /WX)
    add_compile_options(/W4)
    add_compile_options(/arch:IA32 /arch:SSE /arch:SSE2 /arch:AVX /arch:AVX2)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    # add_compile_options(-Wall -Wextra -pedantic -Werror -Wunused-function)
    add_compile_options(-Wall -Wextra -pedantic)
    add_compile_options(-mfma4 -mxop -msse2 -msse3 -msse4a -msse4.1 -msse4.2 -mavx -mavx2 -mxsave)
endif()

file(GLOB RAGII_IMAGE_HEADERS include/*.h)
file(GLOB RAGII_IMAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE RAGII_IMAGE_FILTERS_HEADERS include/filters/*.h)
file(GLOB_RECURSE RAGII_IMAGE_FILTERS_SOURCES src/filters/*.cpp)
file(GLOB_RECURSE RAGII_IMAGE_FORMATS_HEADERS include/formats/*.h)

source_group("Headers" FILES ${RAGII_IMAGE_HEADERS})
source_group("Sources" FILES ${RAGII_IMAGE_SOURCES})
source_group("Headers\\filters" FILES ${RAGII_IMAGE_FILTERS_HEADERS})
source_group("Sources\\filters" FILES ${RAGII_IMAGE_FILTERS_SOURCES})
source_group("Headers\\formats" FILES ${RAGII_IMAGE_FORMATS_HEADERS})

add_library(ragii-image STATIC
    ${RAGII_IMAGE_HEADERS}
    ${RAGII_IMAGE_SOURCES}
    ${RAGII_IMAGE_FILTERS_HEADERS}
    ${RAGII_IMAGE_FILTERS_SOURCES}
    ${RAGII_IMAGE_FORMATS_HEADERS}
)

target_include_directories(ragii-image PUBLIC "include")
target_include_directories(ragii-image PUBLIC "include/filters")
target_include_directories(ragii-image PUBLIC "include/formats")
target_include_directories(ragii-image PRIVATE "../libjpeg-turbo")
target_include_directories(ragii-image PRIVATE "${CMAKE_BINARY_DIR}/lib/libjpeg-turbo")
target_include_directories(ragii-image PRIVATE "../libpng")
target_include_directories(ragii-image PRIVATE "${CMAKE_BINARY_DIR}/lib/libpng")
