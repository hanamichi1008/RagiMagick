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

# ==================================================
# コンパイルオプション
# ==================================================

if (MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_compile_options(/W4)
    set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY VS_STARTUP_PROJECT "${CMAKE_PROJECT_NAME}")
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    add_compile_options(-Wall -Wextra -pedantic)
    add_compile_options(-Wno-unused-function -Wno-unused-parameter -Wno-padded -Wno-sign-conversion -Wno-sign-compare)
endif()

source_group("Sources" FILES src/main.cpp)

# ==================================================
# ライブラリのロード
# ==================================================

set(RAGIMAGICK_EXTERNALLIB_DIR "${CMAKE_CURRENT_LIST_DIR}/external_lib")
set(RAGIMAGICK_EXTERNALLIB_LIBJPEG_TURBO_SRC "${RAGIMAGICK_EXTERNALLIB_DIR}/libjpeg-turbo/src")
set(RAGIMAGICK_EXTERNALLIB_LIBJPEG_TURBO_CACHE "${RAGIMAGICK_EXTERNALLIB_DIR}/libjpeg-turbo/build")
set(RAGIMAGICK_EXTERNALLIB_LIBPNG_SRC "${RAGIMAGICK_EXTERNALLIB_DIR}/libpng/src")
set(RAGIMAGICK_EXTERNALLIB_LIBPNG_CACHE "${RAGIMAGICK_EXTERNALLIB_DIR}/libpng/build")
set(RAGIMAGICK_EXTERNALLIB_ZLIB_SRC "${RAGIMAGICK_EXTERNALLIB_DIR}/zlib/src")
set(RAGIMAGICK_EXTERNALLIB_ZLIB_CACHE "${RAGIMAGICK_EXTERNALLIB_DIR}/zlib/build")

# ragii
message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/ragii")
add_subdirectory(lib/ragii)

# ragii-image
message("[${CMAKE_PROJECT_NAME}] start add_subdirectory lib/ragii-image")
add_subdirectory(lib/ragii-image)

# ==================================================
# ライブラリのリンク
# ==================================================
add_executable(RagiMagick src/main.cpp)
if(MSVC)
    target_link_libraries(RagiMagick jpeg-static)
    target_link_libraries(RagiMagick png_static)
    target_link_libraries(RagiMagick zlibstatic)
else()
    target_link_libraries(RagiMagick ${RAGIMAGICK_EXTERNALLIB_LIBJPEG_TURBO_CACHE}/libturbojpeg.a)
    target_link_libraries(RagiMagick ${RAGIMAGICK_EXTERNALLIB_LIBPNG_CACHE}/libpng16.a)
    target_link_libraries(RagiMagick ${RAGIMAGICK_EXTERNALLIB_ZLIB_CACHE}/libz.a)
endif()
target_link_libraries(RagiMagick ragii ragii-image)

# ライブラリのインクルード
target_include_directories(RagiMagick PRIVATE "lib")

# 依存関係を設定
# コードフォーマットが最初に実行されるよう、各ターゲットが RAGIMAGICK_CODE_FORMAT_TARGET_NAME に依存するよう設定している
# ※ ビルド済みでも毎回フォーマットが実行されてしまった。ファイル数が少なくても待ち時間が気になるから無効化しておく。
#add_dependencies(ragii ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(ragii-image ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})
#add_dependencies(RagiMagick ${RAGIMAGICK_CODE_FORMAT_TARGET_NAME})

# user config
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/config/UserConfig.cmake")
    include(config/UserConfig.cmake)
    message("UserConfig.cmake loaded.")
endif()

# zlib 後始末
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/external_lib/zlib/src/zconf.h.included")
    file(RENAME "./external_lib/zlib/src/zconf.h.included" "./external_lib/zlib/src/zconf.h")
endif()

# libpng コンパイルに必須のファイルが生成されない問題への応急処置
if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/external_lib/libpng/build/pnglibconf.h")
    file(COPY "./external_lib/libpng/src/scripts/pnglibconf.h.prebuilt" DESTINATION "./external_lib/libpng/build")
    file(RENAME "./external_lib/libpng/build/pnglibconf.h.prebuilt" "./external_lib/libpng/build/pnglibconf.h")
endif()
