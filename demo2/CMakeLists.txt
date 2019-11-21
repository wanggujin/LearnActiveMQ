#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
aux_source_directory(. SRCS)
add_executable(demo_runnable ${SRCS})
target_link_libraries(demo_runnable activemq-cpp)