cmake_minimum_required(VERSION 3.14)
add_subdirectory(filters)
add_library(ragii-image
  Bitmap.cpp
  BitmapConverter.cpp
)
target_link_libraries(ragii-image ragii-filters)
