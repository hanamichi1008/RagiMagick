cmake_minimum_required(VERSION 3.14)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(RagiMagick CXX)

# code format
include(CodeFormat.cmake)
set(RAGIMAGICK_CODE_FORMAT_TARGET_NAME "${CMAKE_PROJECT_NAME}_code-format")
format_code(${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})

if (MSVC)
    add_compile_options(/W4 /WX)
    set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT "${CMAKE_PROJECT_NAME}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

source_group("Sources" FILES src/main.cpp)

add_subdirectory(lib/ragii)
add_subdirectory(lib/ragii-image)

add_executable(RagiMagick src/main.cpp)
target_link_libraries(RagiMagick ragii)
target_link_libraries(RagiMagick ragii-image)

target_include_directories(RagiMagick PRIVATE "lib")

# 依存関係を設定
# コードフォーマットが最初に実行されるよう、各ターゲットが RAGIMAGICK_CODE_FORMAT_TARGET_NAME に依存するよう設定している
# ※ ビルド済みでも毎回フォーマットが実行されてしまった。ファイル数が少なくても待ち時間が気になるから無効化しておく。
#add_dependencies(ragii ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(ragii-image ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(RagiMagick ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})