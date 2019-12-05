cmake_minimum_required(VERSION 2.8)
project(Lesson1)

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

add_executable(sender sender.cpp)
target_link_libraries(sender activemq-cpp)

add_executable(receiver receiver.cpp)
target_link_libraries(receiver activemq-cpp)

add_executable(receiver_listener receiver_message_listener.cpp)
target_link_libraries(receiver_listener activemq-cpp)
