# - vars
set(SOURCE_FILES window.cpp sprite.cpp)

# - find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

# - add lib
add_library(Window ${SOURCE_FILES})

# - link libs
target_link_libraries(
  Window
  PUBLIC SDL2::SDL2
  PUBLIC SDL2_ttf::SDL2_ttf
  PUBLIC SDL2_image::SDL2_image
)
