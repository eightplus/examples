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

#define LOG_TAG "Kydroidd"

#include "message_processor.h"
#include "android_api_skeleton.h"

#include "common/template_message_processor.h"

#include "common/kydroid_rpc.pb.h"
//#include "kydroid_bridge.pb.h"

namespace kydroid {
MessageProcessor::MessageProcessor(const std::shared_ptr<network::MessageSender> &sender,
                                   const std::shared_ptr<rpc::PendingCallCache> &pending_calls,
                                   const std::shared_ptr<AndroidApiSkeleton> &platform_api) :
    rpc::MessageProcessor(sender, pending_calls),
    platform_api_(platform_api) {
}

MessageProcessor::~MessageProcessor() {
}

//用户操作set_focused_task时，会通过callmethod调用到这里
void MessageProcessor::dispatch(rpc::Invocation const& invocation) {
  printf("dispatch...........\n");
  if (invocation.method_name() == "launch_application")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::launch_application, invocation);
  else if (invocation.method_name() == "set_focused_task")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::set_focused_task, invocation);
  else if (invocation.method_name() == "remove_task")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::remove_task, invocation);
  else if (invocation.method_name() == "resize_task")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::resize_task, invocation);
  else if (invocation.method_name() == "wakeup_window_task")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::wakeup_window_task, invocation);
  else if (invocation.method_name() == "setprop_window_task")
    invoke(this, platform_api_.get(), &AndroidApiSkeleton::setprop_window_task, invocation);
}

void MessageProcessor::process_event_sequence(const std::string&) {
}
} // namespace network
