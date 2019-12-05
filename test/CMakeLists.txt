cmake_minimum_required(VERSION 2.8)
project(Test)

add_definitions(-std=c++11)
add_definitions("-Wall -g")

include_directories(
#	/opt/activemqcpp/include
  /home/wanggujin/activemqcpp/include
	/usr/local/include/apr-1
)

link_directories(
#	/opt/activemqcpp/lib
  /home/wanggujin/activemqcpp/lib
)

add_executable(sender sender.cpp)
target_link_libraries(sender activemq-cpp)

add_executable(receiver receiver.cpp)
target_link_libraries(receiver activemq-cpp)
