# ~ add target
add_library(
  AdvMath
  vec3.cpp
  matrix4x4.cpp
  )

# ~ include directories
include_directories(
  AdvMath
  PUBLIC ${CMAKE_SOURCE_DIR}/include
  )
