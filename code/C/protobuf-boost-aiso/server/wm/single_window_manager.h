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

#ifndef ANBOX_WM_SINGLE_WINDOW_MANAGER_H_
#define ANBOX_WM_SINGLE_WINDOW_MANAGER_H_

//#include "mywindow.h"
#include "kydroidtask.h"
#include "kydroidrect.h"
#include <map>
#include <memory>
#include <mutex>


class KydroidPlatform;

namespace kydroid {
namespace bridge {
class AndroidApiStub;
} // namespace bridge

namespace platform {
class MyWindow;
}
namespace wm {
//class Window;
class SingleWindowManager/* : public Manager */{
 public:
  SingleWindowManager(/*const std::weak_ptr<KydroidPlatform> &platform,*/
                     const std::shared_ptr<kydroid::bridge::AndroidApiStub> &android_api_stub,
                      const KydroidRect &window_size);
  ~SingleWindowManager();

  void setup();

  //void apply_window_state_update(const WindowState::List &updated, const WindowState::List &removed) override;

  std::shared_ptr<platform::MyWindow> find_window_for_task(const KydroidTask::Id &task);

  void resize_task(const KydroidTask::Id &task, const KydroidRect &rect,
                   const std::int32_t &resize_mode);
  void set_focused_task(const KydroidTask::Id &task);
  void remove_task(const KydroidTask::Id &task);

    void wakeup_window();
    void setprop_window(const std::string &key, const std::string &value);

 private:
  //std::mutex mutex_;
  //std::weak_ptr<KydroidPlatform> platform_;
  std::shared_ptr<kydroid::bridge::AndroidApiStub> android_api_stub_;
  KydroidRect window_size_;
  std::shared_ptr<platform::MyWindow> window_;
  //std::map<KydroidTask::Id, std::shared_ptr<Window>> windows_;
};
}  // namespace wm
}  // namespace anbox

#endif
