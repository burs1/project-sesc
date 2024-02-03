# vars
set(SOURCE_FILES
  component.cpp
  entity.cpp
  scene.cpp
  components/transform.cpp)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# add lib 
add_library(Gameplay ${SOURCE_FILES})

# include dir 
target_include_directories(Gameplay PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  Gameplay
  PUBLIC GFX
  PUBLIC Window
  PUBLIC AdvMath
)
