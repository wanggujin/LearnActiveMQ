add_executable(lesson2_consumer1 consumer1.cpp)
target_link_libraries(lesson2_consumer1 activemq-cpp pthread)

add_executable(lesson2_consumer2 consumer2.cpp)
target_link_libraries(lesson2_consumer2 activemq-cpp)

add_executable(lesson2_consumer3 consumer3.cpp)
target_link_libraries(lesson2_consumer3 activemq-cpp)