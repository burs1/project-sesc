# vars
set(
  SOURCE_FILES
  mesh.cpp
  renderer3d.cpp
  components/renderer.cpp
  components/mesh-renderer.cpp
  components/texture-renderer.cpp)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# find packages
find_package(spdlog REQUIRED)

# add lib
add_library(GFX ${SOURCE_FILES})

# include dirs
target_include_directories(GFX PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  GFX
  PUBLIC AdvMath
  PUBLIC Window
  PUBLIC spdlog::spdlog
)
