set(SOURCES
    main.cpp
    client.c
    dll.c)

add_executable(client ${SOURCES})
target_link_libraries(client
    MyCallbackApi dl pthread)
