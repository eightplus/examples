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

#include "local_socket_messenger.h"
#include "socket_helper.h"

#include <boost/system/error_code.hpp>
#include <boost/exception/errinfo_errno.hpp>

namespace kydroid {
namespace network {
LocalSocketMessenger::LocalSocketMessenger(
    std::shared_ptr<boost::asio::local::stream_protocol::socket> const &socket)
    : BaseSocketMessenger(socket) {}


//boost::asio::local::stream_protocol::socket本地连接
LocalSocketMessenger::LocalSocketMessenger(const std::string &path,
                                           const std::shared_ptr<Runtime> &rt)
    : socket_(std::make_shared<boost::asio::local::stream_protocol::socket>(
          rt->service())) {
  boost::system::error_code err;
  socket_->connect(boost::asio::local::stream_protocol::endpoint(path), err);
  if (err) {
	auto msg = "Failed to connect to socket " + path + ":" + err.message();
        BOOST_THROW_EXCEPTION(std::runtime_error(msg));
  }

  setup(socket_);
}

LocalSocketMessenger::~LocalSocketMessenger() {}
}  // namespace network
}  // namespace kydroid
