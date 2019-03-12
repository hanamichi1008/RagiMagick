cmake_minimum_required(VERSION 2.8)
add_subdirectory(filters)
add_library(Image
  Bitmap.cpp
  BitmapConverter.cpp
)
target_link_libraries(Image Filters)
