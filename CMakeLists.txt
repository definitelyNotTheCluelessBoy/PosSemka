cmake_minimum_required(VERSION 3.18)
project(semestralkaClient)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT "boost_1_84_0")
find_package(Boost 1.84.0)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(semestralkaClient main.cpp
        Simulation.cpp
        Cell.cpp
        Socket.cpp)
target_compile_definitions(semestralkaClient PRIVATE _WIN32_WINNT=0x0601)
target_link_libraries(semestralkaClient ${Boost_LIBRARIES})



