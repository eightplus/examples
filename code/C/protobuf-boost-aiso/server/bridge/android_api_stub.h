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

#ifndef KYDROID_BRIDGE_ANDROID_API_STUB_H_
#define KYDROID_BRIDGE_ANDROID_API_STUB_H_

//#include "../do_not_copy_or_move.h"
#include "../common/wait_handle.h"
#include "../wm/kydroidrect.h"

#include <memory>
#include <vector>
#include <string>
#include <core/property.h>//libproperties-cpp-dev

namespace kydroid {
namespace protobuf {
namespace rpc {
class Void;
}  // namespace rpc

namespace bridge {
class ResizeTask;
class WakeupTask;
class SetPropTask;
}  // namespace bridge
}
}

namespace kydroid {
namespace rpc {
class Channel;
}  // namespace rpc

namespace bridge {
class AndroidApiStub/* : public DoNotCopyOrMove*//*kydroid::application::Manager*/ {
public:
    AndroidApiStub();
    ~AndroidApiStub();

    void set_rpc_channel(const std::shared_ptr<rpc::Channel> &channel);
    void reset_rpc_channel();
    void set_focused_task(const std::int32_t &id);
    void wakeup_window();
    void setprop_window(const std::string &key, const std::string &value);
    core::Property<bool>& ready();

private:
    bool ensure_rpc_channel();

    template <typename Response>
    struct Request {
        Request() : response(std::make_shared<Response>()), success(true) {}
        std::shared_ptr<Response> response;
        bool success;
    };

    void focused_task_set(Request<kydroid::protobuf::rpc::Void> *request);
    void task_window_wakeup(Request<kydroid::protobuf::rpc::Void> *request);
    void task_window_setprop(Request<kydroid::protobuf::rpc::Void> *request);

    mutable std::mutex mutex_;
    std::shared_ptr<rpc::Channel> channel_;
    common::WaitHandle set_focused_task_handle_;
    common::WaitHandle wakeup_task_handle_;
    common::WaitHandle setprop_task_handle_;
    core::Property<bool> ready_;
};
}  // namespace bridge
}  // namespace kydroid

#endif
