cmake_minimum_required(VERSION 3.27)

# - project info
project(
  starfighters
  VERSION 1.0.0
  LANGUAGES CXX
)

# - set up
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# - copy assets
file(COPY assets DESTINATION .)

# - vars
set(SOURCE_FILES src/main.cpp)

# - add subdirs
add_subdirectory(src)

# - add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# - link libs
target_link_libraries(${PROJECT_NAME} Engine)
