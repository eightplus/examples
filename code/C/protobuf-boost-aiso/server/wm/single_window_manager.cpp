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

#include "single_window_manager.h"
//#include "kydroidplatform.h"
#include "../bridge/android_api_stub.h"
#include "mywindow.h"

#include <algorithm>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

namespace kydroid {
namespace wm {
SingleWindowManager::SingleWindowManager(/*const std::weak_ptr<KydroidPlatform> &platform,*/
                                       const std::shared_ptr<bridge::AndroidApiStub> &android_api_stub,
                                         const KydroidRect &window_size)
    : /*platform_(platform), */android_api_stub_(android_api_stub), window_size_(window_size) {}

SingleWindowManager::~SingleWindowManager() {}

void SingleWindowManager::setup() {
  /*if (auto p = platform_.lock()) {
    window_ = p->create_window(0);
    if (!window_->attach())
      printf("Failed to attach window to renderer\n");
  } else {
    throw std::runtime_error("Can't create window as we don't have a platform abstraction");
  }*/
}

/*void SingleWindowManager::apply_window_state_update(const WindowState::List &updated, const WindowState::List &removed) {
  (void)updated;
  (void)removed;
}*/

std::shared_ptr<platform::MyWindow> SingleWindowManager::find_window_for_task(const KydroidTask::Id &task) {
  (void)task;
  return window_;

  /*std::lock_guard<std::mutex> l(mutex_);
  for (const auto &w : windows_) {
    if (w.second->task() == task) return w.second;
  }
  return nullptr;*/

}

void SingleWindowManager::resize_task(const KydroidTask::Id &task, const KydroidRect &rect,
                                      const std::int32_t &resize_mode) {
  (void)task;
  (void)rect;
  (void)resize_mode;
//android_api_stub_->resize_task(task, rect, resize_mode);
}

void SingleWindowManager::set_focused_task(const KydroidTask::Id &task) {
  //(void)task;
	android_api_stub_->set_focused_task(task);
}

void SingleWindowManager::remove_task(const KydroidTask::Id &task) {
//android_api_stub_->remove_task(task);
  if (task != 0) {
    printf("Window with invalid task id was closed\n");
    return;
  }

  // FIXME easiest to terminate is sending ourself the terminate signal
  // which will be then processed by the main loop and terminate the whole
  // application.
  kill(getpid(), SIGTERM);
}

void SingleWindowManager::wakeup_window()
{
    android_api_stub_->wakeup_window();
}

void SingleWindowManager::setprop_window(const std::string &key, const std::string &value)
{
    android_api_stub_->setprop_window(key, value);
}


}  // namespace wm
}  // namespace anbox
