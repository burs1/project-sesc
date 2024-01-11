# vars
set(SOURCE_FILES entity.cpp scene.cpp)
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
