# vars
set(SOURCE_FILES renderer3d.cpp mesh.cpp)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# add lib
add_library(GFX ${SOURCE_FILES})

# include dirs
target_include_directories(GFX PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  GFX
  PUBLIC AdvMath
  PUBLIC Window
)
