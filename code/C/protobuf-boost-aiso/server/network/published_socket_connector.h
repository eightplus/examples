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

#ifndef KYDROID_PUBLISHEDSOCKETCONNECTOR_H
#define KYDROID_PUBLISHEDSOCKETCONNECTOR_H

#include <boost/asio/local/stream_protocol.hpp>

//#include "../do_not_copy_or_move.h"
#include "../runtime.h"
#include "connection_creator.h"

namespace kydroid {
namespace network {
class PublishedSocketConnector/* : public DoNotCopyOrMove*//*, public Connector*/ {
public:
    explicit PublishedSocketConnector(
      const std::string& socket_file, const std::shared_ptr<Runtime>& rt,
      const std::shared_ptr<ConnectionCreator<
          boost::asio::local::stream_protocol>>& connection_creator);
    ~PublishedSocketConnector() noexcept;

    std::string socket_file() const { return socket_file_; }

private:
    void start_accept();
    void on_new_connection(
      std::shared_ptr<boost::asio::local::stream_protocol::socket> const& socket,
      boost::system::error_code const& err);

    const std::string socket_file_;
    std::shared_ptr<Runtime> runtime_;
    std::shared_ptr<ConnectionCreator<boost::asio::local::stream_protocol>> connection_creator_;

    //kobe:注意，如果路径socket_path= "/run/user/1000/kydroid/sockets"不存在，则会报错退出，测试时需要手动创建该目录   /run/user/1000/snap.kydroid/kydroid/sockets
    //error：    terminate called after throwing an instance of 'boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::system::system_error> >' what():  bind: 没有那个文件或目录
    boost::asio::local::stream_protocol::acceptor acceptor_;
};
}  // namespace network
}  // namespace kydroid

#endif
