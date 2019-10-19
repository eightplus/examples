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

#include "platform_message_processor.h"
#include "platform_api_skeleton.h"
#include "../protobuf/kydroid_bridge.pb.h"


namespace kydroid {
namespace bridge {
PlatformMessageProcessor::PlatformMessageProcessor(
    const std::shared_ptr<kydroid::network::MessageSender> &sender,
    const std::shared_ptr<PlatformApiSkeleton> &server,
    const std::shared_ptr<kydroid::rpc::PendingCallCache> &pending_calls)
    : kydroid::rpc::MessageProcessor(sender, pending_calls), server_(server)
{
    //PlatformMessageProcessor继承了rpc::MessageProcessor,所以先调用rpc::MessageProcessor的析构函数，再调用PlatformMessageProcessor的析构函数
}

PlatformMessageProcessor::~PlatformMessageProcessor()
{

}

void PlatformMessageProcessor::dispatch(rpc::Invocation const &invocation) {
  /*if (invocation.method_name() == "set_clipboard_data")
    invoke(this, server_.get(), &PlatformApiSkeleton::set_clipboard_data, invocation);
  else if (invocation.method_name() == "get_clipboard_data")
    invoke(this, server_.get(), &PlatformApiSkeleton::get_clipboard_data, invocation);*/
}

//2019 将boost从套接字读取的数据序列转换成对应的protobuf格式的数据 TODO
void PlatformMessageProcessor::process_event_sequence(const std::string &raw_events)
{
    //kobe: EventSequence from kydroid_bridge.proto
    if (raw_events.empty())
	return;

    //printf("PlatformMessageProcessor::process_event_sequence: %s\n", raw_events.c_str());
    kydroid::protobuf::bridge::EventSequence seq;
    if (!seq.ParseFromString(raw_events)) {
        printf("Failed to parse events from raw string\n");
        return;
    }
    printf("PlatformMessageProcessor::process_event_sequence server receive msg from client\n");
    if (seq.has_boot_finished()) {//android/device/generic/goldfish/service/platform_api_stub.cpp发送对应的数据过来
        server_->handle_boot_finished_event(seq.boot_finished());
    }

    /*if (seq.has_window_state_update()) {
        server_->handle_window_state_update_event(seq.window_state_update());//kobe://子线程 this will lead to :QApplication: Object event filter cannot be in a different thread.
    }

    if (seq.has_application_list_update()) {//kobe:需要将~/aosp/AppStream/device/phytium/ft1500/appmgr/src/org/anbox/appmgr/PlatformService.java中，SERVICE_NAME的值改为"org.kydroid.PlatformService"，DESCRIPTOR的值改为"org.kydroid.IPlatformService"
        server_->handle_application_list_update_event(seq.application_list_update());
    }*/
}
}  // namespace kydroid
}  // namespace network
