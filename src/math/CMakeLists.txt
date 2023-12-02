# - VARIABLES -
set(SOURCE_FILES matrix4x4.cpp vec2.cpp vec3.cpp)
set(INCLUDE_DIRS .)

# - CREATE LIBRARY -
add_library(EngineMath ${SOURCE_FILES})

# - INCLUDE DIRECTORIES - 
target_include_directories(EngineMath PUBLIC ${INCLUDE_DIRS})
