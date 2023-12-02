# vars
set(SOURCE_FILES renderer.cpp mesh.cpp)
set(INCLUDE_DIRS . ../)

# add lib
add_library(Gfx ${SOURCE_FILES})

# include dirs
target_include_directories(Gfx PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  Gfx
  PUBLIC EngineMath
  PUBLIC Window
)
