/*
 * Copyright (C) 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "message_processor.h"
#include "variable_length_array.h"
#include "constants.h"
#include "make_protobuf_object.h"
#include "template_message_processor.h"

#include "kydroid_rpc.pb.h"



/*
anbox::rpc::MessageProcessor 是一个同时用于 RPC 调用发起端和接受端的组件。容器管理器作为 RPC 调用的接受端，接收发自于会话管理器的类型为 MessageType::invocation 的消息。

会话管理器与容器管理器之间的 RPC 通信的消息格式为：[3 个字节的消息头] + [经由 Protobuf MessageLite 对象序列化得到的消息体]，其中消息头的前两个字节为 16 位的消息体长度的大尾端表示，第 3 个字节为消息的类型。RPC 消息的具体定义在 anbox/src/anbox/protobuf/anbox_rpc.proto 文件中
Invocation 消息用于发起 RPC 调用，Result、Void 和 StructuredError 用于返回响应或错误消息。

对于容器管理器而言，anbox::rpc::MessageProcessor 在其 process_data() 中首先提取消息头，得到消息体的长度和类型，然后提取消息体并反序列化得到 Protobuf 消息 anbox::protobuf::rpc::Invocation，随后将该 Protobuf 消息封装为 anbox::rpc::Invocation 类的对象，并调用 dispatch(Invocation const&) 将消息派发出去。

anbox::rpc::Invocation 类的定义（位于 anbox/src/anbox/rpc/message_processor.h 中）

anbox::rpc::Invocation 类只是对 anbox::protobuf::rpc::Invocation 的简单包装。

anbox::rpc::MessageProcessor 的 dispatch(Invocation const&) 是一个虚函数，其实际的实现位于 ManagementApiMessageProcessor 中。anbox::container::ManagementApiMessageProcessor 的定义（位于 anbox/src/anbox/container/management_api_message_processor.h 中）

anbox::container::ManagementApiMessageProcessor 的实现很简单，只支持两种 RPC 调用，分别为启动 Android 容器和停止 Android 容器，在它的 dispatch() 函数中，根据方法名，调用对应的函数。

函数调用通过一个函数模板 invoke() 完成，该函数模板定义（位于 anbox/src/anbox/rpc/template_message_processor.h）

直接启动和停止 Android 容器的职责，由 anbox::container::ManagementApiSkeleton 完成
anbox::container::ManagementApiSkeleton 通过 Container 类启动或停止 Android 容器。配合函数模板 invoke() 的定义，及 Protobuf 的相关方法实现，不难理解， start_container() 和 stop_container() 函数的参数消息，在 invoke() 函数中由 Invocation 消息的参数字段的字节数组反序列化得到，这两个函数的执行过程，都是向 response 参数中填入返回给调用者的响应，并通过 done->Run() 将响应通过 ManagementApiMessageProcessor::send_response() 函数，即
anbox::rpc::MessageProcessor::send_response() 函数发送回调用端。

在 anbox::rpc::MessageProcessor::send_response() 函数中，先将响应序列化，然后将序列化之后的响应放进 anbox::protobuf::rpc::Result Protobuf 消息中，最后再将 anbox::protobuf::rpc::Result 包装为 Anbox 的 RPC 消息发送回调用端。

anbox::container::ManagementApiSkeleton 的 pending_calls_ 似乎也没有实际的用处。
*/

#define LOG_TAG "Kydroidd"

namespace kydroid {
namespace rpc {
const ::std::string &Invocation::method_name() const {
  return invocation_.method_name();
}

const ::std::string &Invocation::parameters() const {
  return invocation_.parameters();
}

google::protobuf::uint32 Invocation::id() const { return invocation_.id(); }

MessageProcessor::MessageProcessor(
    const std::shared_ptr<network::MessageSender> &sender,
    const std::shared_ptr<PendingCallCache> &pending_calls)
    : sender_(sender), pending_calls_(pending_calls) {}

MessageProcessor::~MessageProcessor() {}

bool MessageProcessor::process_data(const std::vector<std::uint8_t> &data) {
  for (const auto &byte : data) buffer_.push_back(byte);
  while (buffer_.size() > 0) {
    const auto high = buffer_[0];
    const auto low = buffer_[1];
    size_t const message_size = (high << 8) + low;
    const auto message_type = buffer_[2];

    // If we don't have yet all bytes for a new message return and wait
    // until we have all.
    if (buffer_.size() - header_size < message_size) break;
    printf("MessageProcessor::process_data==========\n");

    if (message_type == MessageType::invocation) {//server端没有走这里，客户端收到server通过callmethod调用的方法发送的信息后走了这里
      printf("client will call dispatch, and message_type is MessageType::invocation\n");
      kydroid::protobuf::rpc::Invocation raw_invocation;
      raw_invocation.ParseFromArray(buffer_.data() + header_size, message_size);

      dispatch(Invocation(raw_invocation));//dispatch()函数实现在src/message_processor.cpp中
    } else if (message_type == MessageType::response) {//客户端没有走这里，只有server端收到client端发送的消息后走了这里
      printf("MessageType::response..........\n");
      auto result = make_protobuf_object<protobuf::rpc::Result>();
      result->ParseFromArray(buffer_.data() + header_size, message_size);

      if (result->has_id()) {
        pending_calls_->populate_message_for_result(*result,
                                                    [&](google::protobuf::MessageLite *result_message) {
                                                      result_message->ParseFromString(result->response());
                                                    });
        pending_calls_->complete_response(*result);
      }

      for (int n = 0; n < result->events_size(); n++)
        process_event_sequence(result->events(n));
    }

    buffer_.erase(buffer_.begin(),
                  buffer_.begin() + header_size + message_size);
  }

  return true;
}

//client发送数据给server: 由template_message_processor.h中的函数模板invoke()调用
//send_response(): 先将响应序列化，然后将序列化之后的响应放进kydroid::protobuf::rpc::Result Protobuf 消息中，最后再将kydroid::protobuf::rpc::Result 包装为 RPC 消息发送回调用端
void MessageProcessor::send_response(::google::protobuf::uint32 id,
                                     google::protobuf::MessageLite *response) {
  VariableLengthArray<serialization_buffer_size> send_response_buffer(
      static_cast<size_t>(response->ByteSize()));
  printf("MessageProcessor::send_response=====\n");
  response->SerializeWithCachedSizesToArray(send_response_buffer.data());

  kydroid::protobuf::rpc::Result send_response_result;
  send_response_result.set_id(id);
  send_response_result.set_response(send_response_buffer.data(),
                                    send_response_buffer.size());

  send_response_buffer.resize(send_response_result.ByteSize());
  send_response_result.SerializeWithCachedSizesToArray(
      send_response_buffer.data());

  const size_t size = send_response_buffer.size();
  const unsigned char header_bytes[header_size] = {
      static_cast<unsigned char>((size >> 8) & 0xff),
      static_cast<unsigned char>((size >> 0) & 0xff), MessageType::response,
  };

  std::vector<std::uint8_t> send_buffer(sizeof(header_bytes) + size);
  std::copy(header_bytes, header_bytes + sizeof(header_bytes),
            send_buffer.begin());
  std::copy(send_response_buffer.data(),
            send_response_buffer.data() + send_response_buffer.size(),
            send_buffer.begin() + sizeof(header_bytes));

  sender_->send(reinterpret_cast<const char *>(send_buffer.data()),
                send_buffer.size());
}
}  // namespace kydroid
}  // namespace network
