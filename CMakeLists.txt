
cmake_minimum_required(VERSION 3.8)

project(Integrators2 VERSION 0.0 LANGUAGES CXX)

find_package(ROOT REQUIRED COMPONENTS Core RIO Net)

set(CMAKE_CXX_STANDARD
    17
    CACHE STRING "C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_PLATFORM_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(sources 
    MontecarloIntegrate.cpp
    compute_unitball_volume.cpp
    compute_sphere_overlap.cpp
)

set(include 
    MontecarloIntegrate.hpp
    compute_unitball_volume.hpp
    compute_sphere_overlap.hpp
    ValueWithError.hpp
)

#-------------------------------------------------
# 
#   the 'ndcrescent' executable allows computation of a n-ball (using stone throwing) via command-line args
#   
add_executable(ndcrescent ndcrescent.cpp ${sources} ${include})
target_include_directories(ndcrescent PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${ROOT_INCLUDE_DIRS}")
target_link_libraries(ndcrescent PUBLIC ROOT::Core "${ROOT_LIBRARIES}")