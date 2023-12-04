# vars
set(SOURCE_FILES matrix4x4.cpp vec2.cpp vec3.cpp)
set(INCLUDE_DIRS .)

# add lib
add_library(EngineMath ${SOURCE_FILES})

# include dirs 
target_include_directories(EngineMath PUBLIC ${INCLUDE_DIRS})
