include_directories(
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/server
)

protobuf_generate_cpp(
    GENERATED_PROTOBUF_RPC_SRCS GENERATED_PROTOBUF_RPC_HDRS
    protobuf/kydroid_rpc.proto)

protobuf_generate_cpp(
    GENERATED_PROTOBUF_BRIDGE_SRCS GENERATED_PROTOBUF_BRIDGE_HDRS
    protobuf/kydroid_bridge.proto)

add_library(server-protobuf
    STATIC
    ${GENERATED_PROTOBUF_RPC_SRCS}
    ${GENERATED_PROTOBUF_RPC_HDRS}
    ${GENERATED_PROTOBUF_BRIDGE_SRCS}
    ${GENERATED_PROTOBUF_BRIDGE_HDRS}
    protobuf/kydroid_rpc.proto
    protobuf/kydroid_bridge.proto)
target_link_libraries(server-protobuf
    ${PROTOBUF_LITE_LIBRARIES})

set(SOURCES
    runtime.cpp
    runtime.h
    wm/kydroidtask.h
    wm/kydroidrect.h
    wm/kydroidrect.cpp
    common/fd.h
    network/message_receiver.h
    network/message_sender.h
    network/connections.h
    network/message_processor.h
    network/published_socket_connector.h
    network/connection_creator.h
    network/socket_helper.h
    network/local_socket_messenger.h
    network/base_socket_messenger.h
    network/socket_connection.h
    common/variable_length_array.h
    bridge/android_api_stub.h
    bridge/android_api_stub.cpp
    common/wait_handle.h
    rpc/channel.h
    rpc/constants.h
    rpc/pending_call_cache.h
    rpc/connection_creator.h
    rpc/message_processor.h
    rpc/make_protobuf_object.h
    bridge/platform_message_processor.h
    protobuf/kydroid_rpc.pb.h
    protobuf/kydroid_bridge.pb.h
    network/delegate_connection_creator.h
    wm/kydroidtask.cpp
    common/fd.cpp
    network/published_socket_connector.cpp
    network/socket_helper.cpp
    network/local_socket_messenger.cpp
    network/base_socket_messenger.cpp
    network/socket_connection.cpp
    common/wait_handle.cpp
    rpc/channel.cpp
    rpc/pending_call_cache.cpp
    rpc/connection_creator.cpp
    rpc/message_processor.cpp
    bridge/platform_message_processor.cpp
    wm/single_window_manager.cpp
    wm/single_window_manager.h
    wm/mywindow.cpp
    wm/mywindow.h
    bridge/platform_api_skeleton.cpp
    bridge/platform_api_skeleton.h
)

add_library(server-core STATIC ${SOURCES})
target_link_libraries(server-core
  ${Boost_LDFLAGS}
  ${Boost_LIBRARIES}
  pthread
  server-protobuf)

add_executable(server main.cpp)
target_link_libraries(server
    server-core)

install(
  TARGETS server
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static)
