# vars
set(SOURCE_FILES entity.cpp scene.cpp camera.cpp)
set(INCLUDE_DIRS . ../gfx ../math ../window)

# add lib 
add_library(Gameplay ${SOURCE_FILES})

# include dir 
target_include_directories(Gameplay PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  Gameplay
  PUBLIC Gfx
  PUBLIC Window
  PUBLIC EngineMath
)
