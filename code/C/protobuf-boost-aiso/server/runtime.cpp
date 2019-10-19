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

#include <iostream>

#include "runtime.h"

namespace {
// exception_safe_run runs service, catching all exceptions and
// restarting operation until an explicit shutdown has been requested.
//
// TODO(tvoss): Catching all exceptions is risky as they might signal
// unrecoverable
// errors. We should enable calling code to decide whether an exception should
// be considered
// fatal or not.
void exception_safe_run(boost::asio::io_service& service) {
  while (true) {
    try {
      service.run();//run()和poll()都循环执行I/O对象的事件，区别在于如果事件没有被触发（ready），run()会等待，但是poll()会立即返回。也就是说poll()只会执行已经触发的I/O事件。
      // a clean return from run only happens in case of
      // stop() being called (we are keeping the service alive with
      // a service::work instance).
      break;
    } catch (const std::exception& e) {
      printf("kobe exception_safe_run exception:%s\n", e.what());//kobe exception_safe_run exception:Failed to execute process
    } catch (...) {
      printf("Unknown exception caught while executing boost::asio::io_service\n");
    }
  }
}
}
namespace kydroid {

std::shared_ptr<Runtime> Runtime::create(std::uint32_t pool_size) {
  return std::shared_ptr<Runtime>(new Runtime(pool_size));
}

Runtime::Runtime(std::uint32_t pool_size)
    : pool_size_{pool_size},
      
      #if BOOST_VERSION >= 106600
      service_{static_cast<int>(pool_size_)},
      #else
      service_{pool_size_},
      #endif
      
      strand_{service_},
      keep_alive_{service_} {}

Runtime::~Runtime() noexcept(true) {//noexcept说明：　https://blog.csdn.net/qianqin_2014/article/details/51321631
  try {
    stop();
  } catch (...) {
    // Dropping all exceptions to satisfy the nothrow guarantee.
  }
}

void Runtime::start() {
    //kobe: std::vector<std::thread> workers_
  for (unsigned int i = 0; i < pool_size_; i++)
    workers_.push_back(std::thread{exception_safe_run, std::ref(service_)});
}

void Runtime::stop() {
  service_.stop();

  for (auto& worker : workers_)
    if (worker.joinable())
      worker.join();
}

/*
anbox::Runtime::to_dispatcher_functional() 函数返回一个接收一个函数为参数的函数，返回的这个函数可以将它接收的函数作为一个 task，通过 boost::asio::io_service::strand 提交给 boost::asio::io_service 执行。anbox::Runtime::service() 返回 boost::asio::io_service 用于方便一些 boost I/O 组件，直接向该 io_service 中提交任务
继承自 std::enable_shared_from_this<Runtime> 的 shared_from_this() 可以从 this 指针创建一个指向当前对象的 std::shared_ptr 智能指针。std::enable_shared_from_this<Runtime> 有一个类型为 std::weak_ptr<Runtime> 的成员，该成员在首次创建指向对象的 std::shared_ptr 智能指针的时候会被初始化。shared_from_this() 函数被调用时，会从类型为 std::weak_ptr<Runtime> 的成员变量创建指向当前对象的 std::shared_ptr 智能指针。
*/
std::function<void(std::function<void()>)> Runtime::to_dispatcher_functional() {
  // We have to make sure that we stay alive for as long as
  // calling code requires the dispatcher to work.
  auto sp = shared_from_this();
  return [sp](std::function<void()> task) { sp->strand_.post(task); };
}

boost::asio::io_service& Runtime::service() { return service_; }

}  // namespace kydroid
