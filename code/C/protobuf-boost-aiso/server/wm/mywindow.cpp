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

#include "mywindow.h"

#include <boost/throw_exception.hpp>

namespace {
constexpr const int window_resize_border{30};
constexpr const int top_drag_area{50};
constexpr const int button_size{32};
constexpr const int button_margin{5};
constexpr const int button_padding{4};
}

namespace kydroid {
namespace platform {
MyWindow::Id MyWindow::Invalid{-1};

MyWindow::Observer::~Observer() {}

MyWindow::MyWindow(const Id &id, const kydroid::wm::KydroidTask::Id &task/*,
               const std::shared_ptr<Observer> &observer*//*,
               const KydroidRect &frame,
               const std::string &title,
               bool resizable*/)
    : /*kydroid::wm::Window(task),*/
      id_(id) {
      //observer_(observer),
      /*native_display_(0),
      native_window_(0) {*/
}

MyWindow::~MyWindow() {
release();
  //if (window_) SDL_DestroyWindow(window_);
}

void MyWindow::close() {
  //if (observer_)
  //  observer_->window_deleted(id_);
}

void MyWindow::switch_window_state() {
  /*const auto flags = SDL_GetWindowFlags(window_);
  if (flags & SDL_WINDOW_MAXIMIZED)
    SDL_RestoreWindow(window_);
  else
    SDL_MaximizeWindow(window_);*/
}


//EGLNativeWindowType Window::native_handle() const { return native_window_; }

MyWindow::Id MyWindow::id() const { return id_; }


kydroid::wm::KydroidTask::Id MyWindow::task() const { return task_; }

KydroidRect MyWindow::frame() const { return frame_; }

std::string MyWindow::title() const { return title_; }

bool MyWindow::attach() {
  /*if (!renderer_)
    return false;
  attached_ = renderer_->createNativeWindow(native_handle());
  return attached_;*/
  return true;
}

void MyWindow::release() {
  /*if (!renderer_ || !attached_)
    return;
  renderer_->destroyNativeWindow(native_handle());*/
}


//std::uint32_t Window::window_id() const { return SDL_GetWindowID(window_); }
} // namespace platform
} // namespace anbox
