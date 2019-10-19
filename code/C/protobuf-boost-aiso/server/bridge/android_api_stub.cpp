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

#include "android_api_stub.h"
#include "../rpc/channel.h"

#include "../protobuf/kydroid_rpc.pb.h"
#include "../protobuf/kydroid_bridge.pb.h"


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

namespace {
constexpr const std::chrono::milliseconds default_rpc_call_timeout{5000};
} // namespace


//lixiang: AndroidApiStub向client发送数据

namespace kydroid {
namespace bridge {
AndroidApiStub::AndroidApiStub()
{

}

AndroidApiStub::~AndroidApiStub()
{

}

void AndroidApiStub::set_rpc_channel(const std::shared_ptr<rpc::Channel> &channel)
{
    channel_ = channel;
}

void AndroidApiStub::reset_rpc_channel() { channel_.reset(); }

bool AndroidApiStub::ensure_rpc_channel()
{
    //kobe：如果android环境没有启动，即channel_为空，则调用ensure_rpc_channel函数时会抛出异常让程序退出
//    terminate called after throwing an instance of 'std::runtime_error'
//      what():  No remote client connected
    if (!channel_) {
        //TODO: record log
        return false;
        //throw std::runtime_error("No remote client connected");
    }

    return true;
}

core::Property<bool>& AndroidApiStub::ready()
{
    return ready_;
}

void AndroidApiStub::set_focused_task(const std::int32_t &id)
{
    bool b = ensure_rpc_channel();
    if (!b) {
        return;
    }

    auto c = std::make_shared<Request<kydroid::protobuf::rpc::Void>>();

    kydroid::protobuf::bridge::SetFocusedTask message;
    message.set_id(id);

    {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        set_focused_task_handle_.expect_result();
    }

    channel_->call_method("set_focused_task", &message, c->response.get(),
                        google::protobuf::NewCallback(
                            this, &AndroidApiStub::focused_task_set, c.get()));

    set_focused_task_handle_.wait_for_pending(default_rpc_call_timeout);
    if (!set_focused_task_handle_.has_result()) {
        printf("set_focused_task RPC call timed out\n");
        throw std::runtime_error("set_focused_task RPC call timed out");
    }

    if (c->response->has_error()) {
        printf("set_focused_task response has_error\n");
        throw std::runtime_error(c->response->error());
    }
}

void AndroidApiStub::focused_task_set(Request<kydroid::protobuf::rpc::Void> *request)
{
    (void)request;
    set_focused_task_handle_.result_received();
}

void AndroidApiStub::wakeup_window()
{
    bool b = ensure_rpc_channel();
    if (!b) {
        return;
    }

    auto c = std::make_shared<Request<kydroid::protobuf::rpc::Void>>();

    kydroid::protobuf::bridge::WakeupTask message;
    message.set_id(1);

    {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        wakeup_task_handle_.expect_result();
    }

    channel_->call_method("wakeup_window_task", &message, c->response.get(),
                        google::protobuf::NewCallback(
                            this, &AndroidApiStub::task_window_wakeup, c.get()));

//    channel_->call_method("wakeup_window_task", 0, c->response.get(),
//                        google::protobuf::NewCallback(
//                            this, &AndroidApiStub::task_window_wakeup, c.get()));

    wakeup_task_handle_.wait_for_pending(default_rpc_call_timeout);
    if (!wakeup_task_handle_.has_result()) {
//        terminate called after throwing an instance of 'std::runtime_error'
//          what():  wakeup_window RPC call timed out
        //TODO: record log
        printf("wakeup_window RPC call timed out\n");
        //throw std::runtime_error("wakeup_window RPC call timed out");
    }

    if (c->response->has_error()) {
        printf("wakeup_window runtime_error\n");
        //TODO: record log
        //throw std::runtime_error(c->response->error());
    }
}

void AndroidApiStub::task_window_wakeup(Request<kydroid::protobuf::rpc::Void> *request)
{
    (void)request;
    wakeup_task_handle_.result_received();
}

void AndroidApiStub::setprop_window(const std::string &key, const std::string &value)
{
    if (key.empty() || value.empty())
        return;

    bool b = ensure_rpc_channel();
    if (!b) {
        return;
    }

    auto c = std::make_shared<Request<kydroid::protobuf::rpc::Void>>();

    kydroid::protobuf::bridge::SetPropTask message;
    message.set_prop_key(key);
    message.set_prop_value(value);

    {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        setprop_task_handle_.expect_result();
    }

    channel_->call_method("setprop_window_task", &message, c->response.get(),
                        google::protobuf::NewCallback(
                            this, &AndroidApiStub::task_window_setprop, c.get()));

    setprop_task_handle_.wait_for_pending(default_rpc_call_timeout);
    if (!setprop_task_handle_.has_result()) {
        printf("setprop_window RPC call timed out\n");
    }

    if (c->response->has_error()) {
        printf("setprop_window runtime_error\n");
    }
}

void AndroidApiStub::task_window_setprop(Request<kydroid::protobuf::rpc::Void> *request)
{
    (void)request;
    setprop_task_handle_.result_received();
}
}  // namespace bridge
}  // namespace kydroid
