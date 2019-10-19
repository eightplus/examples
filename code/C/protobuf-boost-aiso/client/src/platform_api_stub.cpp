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

#include "platform_api_stub.h"
#include "common/channel.h"

#include "common/kydroid_rpc.pb.h"
#include "common/kydroid_bridge.pb.h"

#include <fstream>

#include <sys/stat.h>

#define LOG_TAG "Kydroidd"


#include <iostream>
using namespace std;

namespace {
constexpr const char *first_boot_marker_path{"/data/.kydroid_initialized"};
}

namespace kydroid {
PlatformApiStub::PlatformApiStub(const std::shared_ptr<rpc::Channel> &rpc_channel) :
    rpc_channel_(rpc_channel) {
}

void PlatformApiStub::boot_finished() {
    printf("PlatformApiStub::boot_finished\n");
    protobuf::bridge::EventSequence seq;
    auto event = seq.mutable_boot_finished();

    struct stat st;
    if (stat(first_boot_marker_path, &st) != 0) {
      event->set_first_boot_done(true);
      std::ofstream marker(first_boot_marker_path);
    }

    rpc_channel_->send_event(seq);
}

void PlatformApiStub::update_window_state(const WindowStateUpdate &state) {
    printf("PlatformApiStub::update_window_state\n");
    protobuf::bridge::EventSequence seq;
    auto event = seq.mutable_window_state_update();

    auto convert_window = [](WindowStateUpdate::Window in, kydroid::protobuf::bridge::WindowStateUpdateEvent_WindowState *out) {
        out->set_display_id(in.display_id);
        out->set_has_surface(in.has_surface);
        out->set_package_name(in.package_name);
        out->set_frame_left(in.frame.left);
        out->set_frame_top(in.frame.top);
        out->set_frame_right(in.frame.right);
        out->set_frame_bottom(in.frame.bottom);
        out->set_task_id(in.task_id);
        out->set_stack_id(in.stack_id);
    };

    for (const auto &window : state.updated_windows) {
        auto w = event->add_windows();
        convert_window(window, w);
    }

    for (const auto &window : state.removed_windows) {
        auto w = event->add_removed_windows();
        convert_window(window, w);
    }

    rpc_channel_->send_event(seq);
}

void PlatformApiStub::update_application_list(const ApplicationListUpdate &update) {
    printf("PlatformApiStub::update_application_list\n");
    protobuf::bridge::EventSequence seq;
    auto event = seq.mutable_application_list_update();

    for (const auto &a : update.applications) {
        auto app = event->add_applications();
        app->set_name(a.name);
        app->set_package(a.package);

        auto launch_intent = app->mutable_launch_intent();
        launch_intent->set_action(a.launch_intent.action);
        launch_intent->set_uri(a.launch_intent.uri);
        launch_intent->set_type(a.launch_intent.type);
        launch_intent->set_package(a.launch_intent.package);
        launch_intent->set_component(a.launch_intent.component);
        for (const auto &category : a.launch_intent.categories) {
            auto c = launch_intent->add_categories();
            *c = category;
        }

        //app->set_icon(a.icon.data(), a.icon.size());//add by kobe
    }

    for (const auto &package : update.removed_applications) {
      auto app = event->add_removed_applications();
      app->set_name("unknown");
      app->set_package(package);
    }

    rpc_channel_->send_event(seq);
}

void PlatformApiStub::on_clipboard_data_set(Request<protobuf::rpc::Void> *request) {
    request->wh.result_received();
}

void PlatformApiStub::set_clipboard_data(const ClipboardData &data) {
    printf("PlatformApiStub::set_clipboard_data\n");
    auto c = std::make_shared<Request<protobuf::rpc::Void>>();

    protobuf::bridge::ClipboardData message;
    message.set_text(data.text);

    {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      c->wh.expect_result();
    }

    rpc_channel_->call_method("set_clipboard_data", &message, c->response.get(),
                              google::protobuf::NewCallback(
                                  this, &PlatformApiStub::on_clipboard_data_set, c.get()));

    c->wh.wait_for_all();

    if (c->response->has_error()) throw std::runtime_error(c->response->error());
}

void PlatformApiStub::on_clipboard_data_get(Request<protobuf::bridge::ClipboardData> *request) {
    request->wh.result_received();
}

PlatformApiStub::ClipboardData PlatformApiStub::get_clipboard_data() {
    printf("PlatformApiStub::get_clipboard_data\n");
    auto c = std::make_shared<Request<protobuf::bridge::ClipboardData>>();

    protobuf::rpc::Void message;

    received_clipboard_data_ = ClipboardData{};

    {
      std::lock_guard<decltype(mutex_)> lock(mutex_);
      c->wh.expect_result();
    }

    rpc_channel_->call_method("get_clipboard_data", &message, c->response.get(),
                              google::protobuf::NewCallback(
                                  this, &PlatformApiStub::on_clipboard_data_get, c.get()));

    c->wh.wait_for_all();

    if (c->response->has_error()) throw std::runtime_error(c->response->error());

    return ClipboardData{c->response->text()};
}
} // namespace kydroid
