cmake_minimum_required(VERSION 3.14)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RagiMagick CXX)

# code format
include(CodeFormat.cmake)
set(RAGIMAGICK_CODE_FORMAT_TARGET_NAME "${CMAKE_PROJECT_NAME}_code-format")
format_code(${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})

if (MSVC)
    add_compile_options(/W4 /WX)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(RagiMagick main.cpp)

add_subdirectory(ragii)
target_link_libraries(RagiMagick ragii)

add_subdirectory(ragii-image)
target_link_libraries(RagiMagick ragii-image)

# 依存関係を設定
# コードフォーマットが最初に実行されるよう、各ターゲットが RAGIMAGICK_CODE_FORMAT_TARGET_NAME に依存するよう設定している
add_dependencies(ragii ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
add_dependencies(ragii-image ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
add_dependencies(RagiMagick ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})