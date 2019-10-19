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

#ifndef ANBOX_PLATFORM_SDL_WINDOW_H_
#define ANBOX_PLATFORM_SDL_WINDOW_H_

#include <memory>
#include <vector>
#include <string>

#include "kydroidrect.h"
#include "kydroidtask.h"

namespace kydroid {
namespace platform {
class MyWindow/* : public std::enable_shared_from_this<MyWindow>*/ {
 public:
  typedef std::int32_t Id;
  static Id Invalid;

  class Observer {
   public:
    virtual ~Observer();
    virtual void window_deleted(const Id &id) = 0;
    virtual void window_wants_focus(const Id &id) = 0;
    virtual void window_moved(const Id &id, const std::int32_t &x,
                              const std::int32_t &y) = 0;
    virtual void window_resized(const Id &id, const std::int32_t &x,
                                const std::int32_t &y) = 0;
  };

typedef std::vector<MyWindow> List;

  MyWindow(const Id &id, const kydroid::wm::KydroidTask::Id &task/*,
         const std::shared_ptr<Observer> &observer*//*,
         const KydroidRect &frame,
         const std::string &title,
         bool resizable*/);
  ~MyWindow();

  Id id() const;
  //std::uint32_t window_id() const;

  bool attach();
  void release();

  KydroidRect frame() const;
  kydroid::wm::KydroidTask::Id task() const;
  std::string title() const;

 private:
  void close();
  void switch_window_state();

  Id id_;
  //std::shared_ptr<Observer> observer_;

  kydroid::wm::KydroidTask::Id task_;
  KydroidRect frame_;
  std::string title_;
  bool attached_ = false;
};
} // namespace platform
} // namespace anbox

#endif
