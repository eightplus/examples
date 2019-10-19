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

#include <signal.h>
#include "host_connector.h"
//#include "platform_service.h"
//#include "event_listener.h"
//#include "mount_cmd_listener.h"
//#include "internal_service.h"

#include "platform_api_stub.h"
#include <memory>
#include <cstdlib>
#include <string.h>
#include <unistd.h>

#include <pthread.h>

std::shared_ptr<kydroid::HostConnector> host_connector;


kydroid::PlatformApiStub::WindowStateUpdate::Window unpack_window_state(int i) {

    return kydroid::PlatformApiStub::WindowStateUpdate::Window{
        -1, // Display id will be added by the caller
        false,
        "aaa",
        {0, 0, 0, 0},
        1,
        2,
    };
}


static void* request_android_thread(void *arg)
{
    std::shared_ptr<kydroid::PlatformApiStub> platform_api_stub_ = host_connector->platform_api_stub();
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
		if (platform_api_stub_) {
			kydroid::PlatformApiStub::WindowStateUpdate state;
			    for (auto n = 0; n < 3; n++) {
				const auto display_id = n;
				const auto num_windows = n;
				for (auto m = 0; m < num_windows; m++) {
				    auto window = unpack_window_state(m);
				    /*auto window = kydroid::PlatformApiStub::WindowStateUpdate::Window{
					-1, // Display id will be added by the caller
					false,
					"aaa"
					{0, 0, 0, 0},
					0,
					1,
				    };
				    window.display_id = display_id;
				    state.updated_windows.push_back(window);*/
				}
			    }

			    const auto num_removed_windows = 2;
			    for (auto n = 0; n < num_removed_windows; n++) {
				    auto window = unpack_window_state(n);
				    /*auto window = kydroid::PlatformApiStub::WindowStateUpdate::Window{
					-1, // Display id will be added by the caller
					false,
					"bbb"
					{0, 0, 0, 0},
					1,
					2,
				    };
				state.removed_windows.push_back(window);*/
			    }
			platform_api_stub_->update_window_state(state);
		}
                break;
            case 2:
		if (platform_api_stub_) {
		    kydroid::PlatformApiStub::ApplicationListUpdate update;
		    const auto num_packages = 2;
		    for (auto n = 0; n < num_packages; n++) {
			auto p = kydroid::PlatformApiStub::ApplicationListUpdate::Application{
				"aaa",
				"bbb",
			};

			std::vector<std::string> categories;
			unsigned int num_categories = 3;
			for (int m = 0; m < num_categories; m++)
			    categories.push_back("ccc");

			p.launch_intent.action = "action";
			p.launch_intent.uri = "uri";
			p.launch_intent.type = "type";
			p.launch_intent.package = "component_package";
			p.launch_intent.component = "component_class";
			p.launch_intent.categories = categories;

			update.applications.push_back(p);
		    }

		    const auto num_removed_packages = 1;
		    for (auto n = 0; n < num_removed_packages; n++) {
		      update.removed_applications.push_back("bbb");
		    }

		    platform_api_stub_->update_application_list(update);
		}
                break;
            case 3:
		if (platform_api_stub_) {
			    const auto clip_data = kydroid::PlatformApiStub::ClipboardData{"aaaaaaaaaaaaaaaaaa"};
    			    platform_api_stub_->set_clipboard_data(clip_data);
		}
                break;
            case 4:
		if (platform_api_stub_) {
    			    //kydroid::PlatformApiStub::ClipboardData data = platform_api_stub_->get_clipboard_data();
		}
                break;
            case 5:
		if (platform_api_stub_) {
    			    platform_api_stub_->boot_finished();
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


int main(int, char**) {

    signal(SIGPIPE, SIG_IGN);

    //auto host_connector = std::make_shared<kydroid::HostConnector>();
    host_connector = std::make_shared<kydroid::HostConnector>();
    host_connector->start();

    pthread_t tid;
    pthread_create(&tid, NULL, request_android_thread, NULL);

    while(1) {sleep(1);}

    return 0;
}
