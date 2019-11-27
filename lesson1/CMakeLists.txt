add_executable(sender sender.cpp)
add_executable(receiver receiver.cpp)
target_link_libraries(sender activemq-cpp)
target_link_libraries(receiver activemq-cpp)