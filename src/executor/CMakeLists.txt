SET(EXECUTOR_SOURCE executor.cpp)

ADD_EXECUTABLE(executor ${EXECUTOR_SOURCE})
TARGET_LINK_LIBRARIES(executor libzmq cppzmq shm-ipc rt fmt yaml-cpp dl)