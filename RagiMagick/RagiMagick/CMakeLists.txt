cmake_minimum_required(VERSION 3.14)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RagiMagick CXX)

add_executable(RagiMagick main.cpp)

if (MSVC)
    # 効かない・・・
    # add_compile_options(/W4)
endif()

# 効かない・・・
# target_compile_options(
#     "${CMAKE_PROJECT_NAME}" PRIVATE
#     $<$<CXX_COMPILER_ID:Clang>:-Wall>
#     $<$<CXX_COMPILER_ID:AppleClang>:-Wall>
#     $<$<CXX_COMPILER_ID:GNU>:-Wall>
#     $<$<CXX_COMPILER_ID:MSVC>:/W4>
# )

add_subdirectory(ragii)
target_link_libraries(RagiMagick ragii-image)
