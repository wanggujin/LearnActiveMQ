add_executable(topic_sender sender.cpp)
target_link_libraries(topic_sender activemq-cpp)

add_executable(topic_receiver receiver.cpp)
target_link_libraries(topic_receiver activemq-cpp)