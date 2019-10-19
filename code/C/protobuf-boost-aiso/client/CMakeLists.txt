include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
)

set(SOURCES
    src/main.cpp
    src/host_connector.cpp
    src/local_socket_connection.cpp
    src/platform_api_stub.cpp
    src/platform_api_stub.h
    src/message_processor.cpp
    src/common/fd.cpp
    src/common/wait_handle.cpp
    src/common/message_processor.cpp
    src/common/pending_call_cache.cpp
    src/common/channel.cpp
    src/common/kydroid_bridge.pb.h
    src/common/kydroid_rpc.pb.h
    src/common/kydroid_bridge.pb.cc
    src/common/kydroid_rpc.pb.cc
    src/android_api_skeleton.h
    src/android_api_skeleton.cpp
)

add_executable(client ${SOURCES})
target_link_libraries(client
    pthread
    ${PROTOBUF_LITE_LIBRARIES})

install(
  TARGETS client
  RUNTIME DESTINATION bin)
