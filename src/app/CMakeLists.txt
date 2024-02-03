# vars
set(SOURCE_FILES app.cpp)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

# find package
find_package(spdlog REQUIRED)

# add lib 
add_library(App ${SOURCE_FILES})

# include dir 
target_include_directories(App PUBLIC ${INCLUDE_DIRS})

# link libs
target_link_libraries(
  App
  PUBLIC GFX
  PUBLIC Window
  PUBLIC Gameplay
  PUBLIC spdlog::spdlog
)
