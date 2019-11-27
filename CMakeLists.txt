cmake_minimum_required(VERSION 2.8)
project(LearnActiveMQ)

add_definitions(-std=c++11)
add_definitions("-Wall -g")

include_directories(
	/opt/activemqcpp/include
	/usr/local/include/apr-1
)

link_directories(
	/opt/activemqcpp/lib
)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
add_subdirectory(lesson1)
add_subdirectory(lesson2)
add_subdirectory(lesson3)
add_subdirectory(lesson4)