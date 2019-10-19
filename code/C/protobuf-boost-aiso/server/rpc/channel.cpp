/*
 * Copyright © 2012 Canonical Ltd.
 *           © 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#include "channel.h"
#include "../common/variable_length_array.h"
#include "../network/message_sender.h"
#include "constants.h"
#include "pending_call_cache.h"
#include "../protobuf/kydroid_rpc.pb.h"//for google::protobuf::MessageLite

namespace kydroid {
namespace rpc {
Channel::Channel(const std::shared_ptr<PendingCallCache> &pending_calls,
                 const std::shared_ptr<network::MessageSender> &sender)
    : pending_calls_(pending_calls), sender_(sender) {
}

Channel::~Channel() {}

void Channel::call_method(std::string const &method_name,
                          google::protobuf::MessageLite const *parameters,
                          google::protobuf::MessageLite *response,
                          google::protobuf::Closure *complete) {
    //kobe:此处调用的函数在kydroidd的源文件/android/service/android_api_skeleton.cpp中进行对应的接受处
    printf("server call_method: %s\n", method_name.c_str());
    auto const &invocation = invocation_for(method_name, parameters);
    //if (method_name != std::string("wakeup_window_task")) {//Attention:需要屏蔽wakeup_window_task，否则调用安卓调用唤醒操作后，kydroid-appstream程序挂掉
        pending_calls_->save_completion_details(invocation, response, complete);
    //}
  send_message(MessageType::invocation, invocation);//kobe:x11 send info to kydroidd
}

//TODO
void Channel::send_event(google::protobuf::MessageLite const &event) {
  printf("server send_event\n");
  /*VariableLengthArray<2048> buffer{static_cast<size_t>(event.ByteSize())};
  event.SerializeWithCachedSizesToArray(buffer.data());

  kydroid::protobuf::rpc::Result response;
  response.add_events(buffer.data(), buffer.size());

  send_message(MessageType::response, response);*/
}

kydroid::protobuf::rpc::Invocation Channel::invocation_for(
    std::string const &method_name,
    google::protobuf::MessageLite const *request) {

    printf("server invocation_for: %s\n", method_name.c_str());
    kydroid::protobuf::rpc::Invocation invoke;

    if (request) {
        kydroid::VariableLengthArray<2048> buffer{
              static_cast<size_t>(request->ByteSize())};
        request->SerializeWithCachedSizesToArray(buffer.data());
        invoke.set_id(next_id());
        invoke.set_method_name(method_name);
        invoke.set_parameters(buffer.data(), buffer.size());
        invoke.set_protocol_version(1);
    }
    else {
      kydroid::VariableLengthArray<2048> buffer{
            static_cast<size_t>(0)};
      invoke.set_id(next_id());
      invoke.set_method_name(method_name);
      invoke.set_parameters(buffer.data(), buffer.size());
      invoke.set_protocol_version(1);
    }

  return invoke;
}

//kobe:x11 send info to kydroidd
void Channel::send_message(const std::uint8_t &type,
                           google::protobuf::MessageLite const &message) {
  printf("server send_message\n");
  const size_t size = message.ByteSize();
  const unsigned char header_bytes[header_size] = {
      static_cast<unsigned char>((size >> 8) & 0xff),
      static_cast<unsigned char>((size >> 0) & 0xff), type,
  };

  //protobuf内存申请，相当于写
  std::vector<std::uint8_t> send_buffer(sizeof(header_bytes) + size);
  std::copy(header_bytes, header_bytes + sizeof(header_bytes),
            send_buffer.begin());
  message.SerializeToArray(send_buffer.data() + sizeof(header_bytes), size);

  try {
    std::lock_guard<std::mutex> lock(write_mutex_);
    sender_->send(reinterpret_cast<const char *>(send_buffer.data()), send_buffer.size());//kobe:调用的时BaseSocketMessenger<stream_protocol>::send();
  } catch (std::runtime_error const &) {
    printf("server send_message error caught\n");
    notify_disconnected();
//    throw;//TODO: remove throw
  }
}

void Channel::notify_disconnected() { pending_calls_->force_completion(); }

std::uint32_t Channel::next_id() {
  static std::uint32_t next_message_id = 0;
  return next_message_id++;
}
}  // namespace rpc
}  // namespace kydroid
