
cmake_minimum_required(VERSION 3.8)

project(Integrators2)

find_package(ROOT REQUIRED COMPONENTS Core RIO Net)

set(sources 
    MontecarloIntegrate.cpp
    compute_unitball_volume.cpp
    integrate.cpp
)

set(include 
    MontecarloIntegrate.hpp
    compute_unitball_volume.hpp
)

add_executable(integrate ${sources} ${include})

target_include_directories(integrate PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${ROOT_INCLUDE_DIRS}")

target_link_libraries(integrate PUBLIC ROOT::Core "${ROOT_LIBRARIES}")