set(SOURCES
    my_callback_api.cpp
    DataController.cpp
    WorkerThread.cpp
    thread.cpp)

add_library(MyCallbackApi SHARED ${SOURCES})
#install(TARGETS MyCallbackApi LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
