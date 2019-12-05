add_executable(sender_sync sender_sync.cpp)
target_link_libraries(sender_sync activemq-cpp)

add_executable(sender_async sender_async.cpp)
target_link_libraries(sender_async activemq-cpp)

add_executable(receiver_async receiver_async.cpp)
target_link_libraries(receiver_async activemq-cpp)