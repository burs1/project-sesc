# vars
set(
  SOURCE_FILES
  audio.cpp
  drawer-sdl.cpp
  drawer.cpp
  input.cpp
  texture.cpp
  window.cpp)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(spdlog REQUIRED)

# add lib
add_library(Window ${SOURCE_FILES})

# include dirs
target_include_directories(Window PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  Window
  PUBLIC SDL2::SDL2
  PUBLIC SDL2_ttf::SDL2_ttf
  PUBLIC SDL2_image::SDL2_image
  PUBLIC SDL2_mixer::SDL2_mixer
  PUBLIC spdlog::spdlog
)
