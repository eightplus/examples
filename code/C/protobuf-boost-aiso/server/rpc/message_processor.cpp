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
#include "../common/variable_length_array.h"
#include "constants.h"
#include "make_protobuf_object.h"
#include "../protobuf/kydroid_rpc.pb.h"



namespace kydroid {
namespace rpc {
const ::std::string &Invocation::method_name() const {
  return invocation_.method_name();
}

const ::std::string &Invocation::parameters() const {
  return invocation_.parameters();
}

google::protobuf::uint32 Invocation::id() const { return invocation_.id(); /*return 0;*/ }

MessageProcessor::MessageProcessor(
    const std::shared_ptr<network::MessageSender> &sender,
    const std::shared_ptr<PendingCallCache> &pending_calls)
    : sender_(sender), pending_calls_(pending_calls) {
}

MessageProcessor::~MessageProcessor() {}

//2019 kobe：解析kydroidd通过套接字使用Channel::send_message发送过来的数据，数据格式需要严格对齐
bool MessageProcessor::process_data(const std::vector<std::uint8_t> &data) {
  for (const auto &byte : data) buffer_.push_back(byte);//std::vector<std::uint8_t> buffer_;
  while (buffer_.size() > 0) {
    const auto high = buffer_[0];
    const auto low = buffer_[1];
    size_t const message_size = (high << 8) + low;
    const auto message_type = buffer_[2];

    // If we don't have yet all bytes for a new message return and wait
    // until we have all.
    if (buffer_.size() - header_size < message_size)  {
        printf(" we don't have yet all bytes for a new message\n");
        break;
    }
    printf("MessageProcessor::process_data==========\n");
    if (message_type == MessageType::invocation) {//server端收到client通过callmethod调用方法时走这里，如set_clipboard_data，客户端收到server通过callmethod调用的方法发送的信息后走了这里
      printf("MessageType::invocation..........\n");
      kydroid::protobuf::rpc::Invocation raw_invocation;
      raw_invocation.ParseFromArray(buffer_.data() + header_size, message_size);
      dispatch(Invocation(raw_invocation));
    }
    else if (message_type == MessageType::response) {//客户端没有走这里，只有server端收到client端发送的消息后走了这里
      printf("MessageType::response..........\n");
      auto result = make_protobuf_object<kydroid::protobuf::rpc::Result>();
      result->ParseFromArray(buffer_.data() + header_size, message_size);

      if (result->has_id()) {
	printf("MessageType::response has id..........\n");
        pending_calls_->populate_message_for_result(*result,
                                                    [&](google::protobuf::MessageLite *result_message) {
                                                      result_message->ParseFromString(result->response());
                                                    });
        pending_calls_->complete_response(*result);
      }

      for (int n = 0; n < result->events_size(); n++) {
	printf("MessageType::responseresult->events_size() > 0, and it will call process_event_sequence..........\n");
        process_event_sequence(result->events(n));//process_event_sequence实现在./bridge/platform_message_processor.cpp中
      }
    }

    buffer_.erase(buffer_.begin(), buffer_.begin() + header_size + message_size);
  }

  return true;
}

//TODO
void MessageProcessor::send_response(::google::protobuf::uint32 id,
                                     google::protobuf::MessageLite *response) {
   printf("MessageProcessor::send_response=====\n");
  /*VariableLengthArray<serialization_buffer_size> send_response_buffer(
      static_cast<size_t>(response->ByteSize()));

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
                send_buffer.size());*/
}
}  // namespace kydroid
}  // namespace network
