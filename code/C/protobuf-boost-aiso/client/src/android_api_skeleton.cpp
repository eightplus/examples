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

#include "android_api_skeleton.h"

#include "./common/kydroid_rpc.pb.h"
#include "./common/kydroid_bridge.pb.h"

//#include <core/posix/exec.h>
//#include <core/posix/child_process.h>

//#include <binder/IServiceManager.h>

#include <string>
#include <sstream>
//#include <cutils/properties.h>

namespace {
std::map<std::string,std::string> common_env = {
    {"ANDROID_DATA", "/data"},
    {"ANDROID_ROOT", "/system"},
    {"PATH", "$PATH:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin"},
    {"BOOTCLASSPATH", "/system/framework/core-libart.jar:/system/framework/conscrypt.jar:/system/framework/okhttp.jar:/system/framework/core-junit.jar:/system/framework/bouncycastle.jar:/system/framework/ext.jar:/system/framework/framework.jar:/system/framework/telephony-common.jar:/system/framework/voip-common.jar:/system/framework/ims-common.jar:/system/framework/mms-common.jar:/system/framework/android.policy.jar:/system/framework/apache-xml.jar"},
    {"SYSTEMSERVERCLASSPATH", "/system/framework/services.jar:/system/framework/ethernet-service.jar:/system/framework/wifi-service.jar"},
};
}

namespace kydroid {
AndroidApiSkeleton::AndroidApiSkeleton() {
}

AndroidApiSkeleton::~AndroidApiSkeleton() {
}

/*void AndroidApiSkeleton::wait_for_process(core::posix::ChildProcess &process,
                                   kydroid::protobuf::rpc::Void *response) {
    const auto result = process.wait_for(core::posix::wait::Flags::untraced);
    if (result.status != core::posix::wait::Result::Status::exited ||
        result.detail.if_exited.status != core::posix::exit::Status::success) {
        response->set_error("Failed to execute process");
        // FIXME once we add proper error codes/domains we need to add structured error
        // info to the response here.
    }
}*/

void AndroidApiSkeleton::connect_services() {
    /*if (!activity_manager_.get()) {
        auto am = android::defaultServiceManager()->getService(android::String16("activity"));
        if (am.get())
            activity_manager_ = new android::BpActivityManager(am);
    }*/
}

void AndroidApiSkeleton::launch_application(kydroid::protobuf::bridge::LaunchApplication const *request,
                                kydroid::protobuf::rpc::Void *response,
                                google::protobuf::Closure *done) {
    (void) response;

    /*auto intent = request->intent();

    std::vector<std::string> argv = {
        "/system/bin/am",
        "start",
    };

    if (request->has_stack()) {
      argv.push_back("--stack");
      argv.push_back(std::to_string(request->stack()));
    }
    //add by kobe 20180503
    if (request->has_launch_bounds()) {
        argv.push_back("--launch-bounds");



        argv.push_back(std::to_string(request->launch_bounds().left()));
        argv.push_back(std::to_string(request->launch_bounds().top()));
        argv.push_back(std::to_string(request->launch_bounds().right()));
        argv.push_back(std::to_string(request->launch_bounds().bottom()));
        //std::stringstream launch_bounds;
        //launch_bounds << request->launch_bounds().left() << " "
        //              << request->launch_bounds().top() << " "
        //              << request->launch_bounds().right() << " "
        //              << request->launch_bounds().bottom();
        //argv.push_back(launch_bounds.str());
    }

    if (intent.has_action()) {
        argv.push_back("-a");
        argv.push_back(intent.action());
    }

    if (intent.has_uri()) {
        argv.push_back("-d");
        argv.push_back(intent.uri());
    }

    if (intent.has_type()) {
        argv.push_back("-t");
        argv.push_back(intent.type());
    }

    std::string component;
    if (intent.has_package())
        component += intent.package();
    if (!component.empty() && intent.has_component()) {
        component += "/";
        component += intent.component();
    }

    if (!component.empty())
        argv.push_back(component);


    auto process = core::posix::exec("/system/bin/sh", argv, common_env, core::posix::StandardStream::empty);
        wait_for_process(process, response);
*/
    done->Run();
}

void AndroidApiSkeleton::set_focused_task(kydroid::protobuf::bridge::SetFocusedTask const *request,
                                          kydroid::protobuf::rpc::Void *response,
                                          google::protobuf::Closure *done) {

   printf("AndroidApiSkeleton::set_focused_task.......\n");
    done->Run();
}

void AndroidApiSkeleton::remove_task(kydroid::protobuf::bridge::RemoveTask const *request,
                                     kydroid::protobuf::rpc::Void *response,
                                     google::protobuf::Closure *done) {

  done->Run();

}

void AndroidApiSkeleton::resize_task(kydroid::protobuf::bridge::ResizeTask const *request,
                                     kydroid::protobuf::rpc::Void *response,
                                     google::protobuf::Closure *done) {
 
  done->Run();
}

//kydroid::protobuf::bridge::RemoveTask无实际作用，只是为了满足模板的参数要求，如果linux段需要传递参数过来，这里可以在kydroid_bridge.proto中增加自定义的类型，如WakeupTask来替换RemoveTask
void AndroidApiSkeleton::wakeup_window_task(kydroid::protobuf::bridge::WakeupTask const *request,
                                     kydroid::protobuf::rpc::Void *response,
                                     google::protobuf::Closure *done) {
    (void) request;
    (void) response;

    done->Run();
}

void AndroidApiSkeleton::setprop_window_task(kydroid::protobuf::bridge::SetPropTask const *request,
                                     kydroid::protobuf::rpc::Void *response,
                                     google::protobuf::Closure *done) {
    (void) response;

    done->Run();
}
} // namespace kydroid
