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


#include "runtime.h"
#include "network/local_socket_messenger.h"
#include "network/published_socket_connector.h"
#include "rpc/connection_creator.h"
#include "bridge/platform_message_processor.h"
#include "bridge/platform_api_skeleton.h"
//#include "wm/kydroidplatform.h"
#include "wm/single_window_manager.h"
#include "rpc/pending_call_cache.h"
#include "bridge/android_api_stub.h"
#include "rpc/channel.h"

#include <pthread.h>

std::shared_ptr<kydroid::wm::SingleWindowManager> window_manager_;

static void* request_android_thread(void* arg)
{
//const std::shared_ptr<kydroid::wm::SingleWindowManager> &window_manager;
//std::shared_ptr<kydroid::wm::SingleWindowManager> window_manager_ = (std::shared_ptr<kydroid::wm::SingleWindowManager>)arg;
    //char *container_name = (char*)arg;
    //开始循环，接收用户输入选择，并响应回复信息
    char buff[256];
    while (fgets(buff, sizeof(buff), stdin) != NULL)
    {
        int iCmdType = 0;
        if(sscanf(buff, "%d", &iCmdType) == -1) {
            fprintf(stderr, "Input error: %s(errno: %d) And please input again\n", strerror(errno), errno);
            continue;
        }
        int op = 0;

        switch(iCmdType)
        {
            case 0:
                printf("request for thread to exit!\n");
                op = 1;
                break;
            case 1:
		if (window_manager_) {
			window_manager_->set_focused_task(0);
		}
                break;
            case 2:
		if (window_manager_) {
			window_manager_->wakeup_window();
		}
                break;
            case 3:
		//std::string key, cstd::string value
		if (window_manager_) {
			window_manager_->setprop_window("aaa", "bbb");
		}
                break;
            default:
                printf("unrecognized command!\n");
                break;
        }

        if(op == 1)
            break;
    }

    return (void*)0;
}

int main(int argc, char **argv)
{
	auto rt = kydroid::Runtime::create();


	auto android_api_stub = std::make_shared<kydroid::bridge::AndroidApiStub>();
	auto display_frame = KydroidRect::Invalid;
	/*auto platform = std::make_shared<KydroidPlatform>(display_frame);
	if (!platform)
		return EXIT_FAILURE;*/

	//std::shared_ptr<kydroid::wm::SingleWindowManager> window_manager_;
	window_manager_ = std::make_shared<kydroid::wm::SingleWindowManager>(/*platform, */android_api_stub, display_frame);
	//platform->set_window_manager(window_manager_);
	window_manager_->setup();

	std::string android_socket_path;
	android_socket_path = "/tmp/my_desktop";
	auto m_socketConnector = std::make_shared<kydroid::network::PublishedSocketConnector>(android_socket_path, rt,
		std::make_shared<kydroid::rpc::ConnectionCreator>(
			//rt, [&](const std::shared_ptr<kydroid::network::LocalSocketMessenger> &) {
            		rt, [&](const std::shared_ptr<kydroid::network::MessageSender> &sender) {


              auto pending_calls = std::make_shared<kydroid::rpc::PendingCallCache>();
              auto rpc_channel =
                  std::make_shared<kydroid::rpc::Channel>(pending_calls, sender);
              // This is safe as long as we only support a single client. If we
              // support
              // more than one one day we need proper dispatching to the right
              // one.
              android_api_stub->set_rpc_channel(rpc_channel);
              auto server = std::make_shared<kydroid::bridge::PlatformApiSkeleton>(
                  /*pending_calls, platform, window_manager_, app_db*/);
              return std::make_shared<kydroid::bridge::PlatformMessageProcessor>(
                  sender, server, pending_calls);

			//return std::make_shared<kydroid::bridge::PlatformMessageProcessor>();
	}));
	rt->start();

    pthread_t tid;
    pthread_create(&tid, NULL, request_android_thread, &window_manager_);

	while (1) {
		sleep(1);
	}
	rt->stop();



	window_manager_.reset();
	//rt->reset();
}
