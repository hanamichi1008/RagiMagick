cmake_minimum_required(VERSION 3.14)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(RagiMagick CXX)

if (MSVC)
    add_compile_options(/W4 /WX)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU|AppleClang)
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(RagiMagick main.cpp)

add_subdirectory(ragii)
target_link_libraries(RagiMagick ragii)

add_subdirectory(ragii-image)
target_link_libraries(RagiMagick ragii-image)
