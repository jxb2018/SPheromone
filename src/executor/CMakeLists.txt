SET(EXECUTOR_SOURCE executor.cpp)

ADD_EXECUTABLE(executor ${EXECUTOR_SOURCE})
TARGET_LINK_LIBRARIES(executor libzmq cppzmq shm-ipc rt fmt yaml-cpp dl)

ADD_EXECUTABLE(executor2 ${EXECUTOR_SOURCE})
TARGET_LINK_LIBRARIES(executor2 libzmq cppzmq shm-ipc rt fmt yaml-cpp dl)