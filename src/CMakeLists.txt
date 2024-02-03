# vars
set(INCLUDE_DIRS window)

# find packages
find_package(spdlog REQUIRED)

# add subdirs
add_subdirectory(app)
add_subdirectory(gfx)
add_subdirectory(gmpl)
add_subdirectory(math)
add_subdirectory(window)

# add lib
add_library(Engine null.cpp)

# include dirs
target_include_directories(Engine PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  Engine
  PUBLIC App
  PUBLIC GFX
  PUBLIC Window
  PUBLIC Gameplay
  PUBLIC AdvMath
  PUBLIC spdlog::spdlog
)
