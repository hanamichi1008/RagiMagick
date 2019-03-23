cmake_minimum_required(VERSION 3.14)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Debug")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UC)

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
    add_compile_options(/W4)
    set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT "${CMAKE_PROJECT_NAME}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

source_group("Sources" FILES src/main.cpp)

# ライブラリのロード
message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/libjpeg-turbo")
add_subdirectory(lib/libjpeg-turbo EXCLUDE_FROM_ALL)

message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/zlib")
add_subdirectory(lib/zlib EXCLUDE_FROM_ALL)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/zlib;${CMAKE_BINARY_DIR}/lib/zlib")
set(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIR}")
if(${CMAKE_BUILD_TYPE_UC} MATCHES "DEBUG")
    set(ZLIB_LIBRARY "${CMAKE_BINARY_DIR}/lib/zlib/Debug/zlibd.lib")
else()
    set(ZLIB_LIBRARY "${CMAKE_BINARY_DIR}/lib/zlib/Release/zlib.lib")
endif()
set(ZLIB_LIBRARIES "${ZLIB_LIBRARY}")
set(ZLIB_FOUND true)

message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/libpng")
add_subdirectory(lib/libpng EXCLUDE_FROM_ALL)
message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/ragii")
add_subdirectory(lib/ragii)
message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/ragii-image")
add_subdirectory(lib/ragii-image)

# ライブラリのリンク
add_executable(RagiMagick src/main.cpp)
target_link_libraries(RagiMagick
    jpeg-static
    png_static
    zlib
    ragii
    ragii-image
)

# ライブラリのインクルード
target_include_directories(RagiMagick PRIVATE "lib")
target_include_directories(RagiMagick PRIVATE "${CMAKE_BINARY_DIR}/lib/libpng")

# 依存関係を設定
# コードフォーマットが最初に実行されるよう、各ターゲットが RAGIMAGICK_CODE_FORMAT_TARGET_NAME に依存するよう設定している
# ※ ビルド済みでも毎回フォーマットが実行されてしまった。ファイル数が少なくても待ち時間が気になるから無効化しておく。
add_dependencies(png_static zlib)
#add_dependencies(ragii ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(ragii-image ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(RagiMagick ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})

# user config
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/config/UserConfig.cmake")
    include(config/UserConfig.cmake)
    message("UserConfig.cmake loaded.")
endif()

# zlib 後始末
file(RENAME "./lib/zlib/zconf.h.included" "./lib/zlib/zconf.h")
