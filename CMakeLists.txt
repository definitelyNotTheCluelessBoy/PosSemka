cmake_minimum_required(VERSION 3.18)
project(semkaServer)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost 1.74.0)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(semkaServer main.cpp
)
target_link_libraries(semkaServer ${Boost_LIBRARIES} pthread)
