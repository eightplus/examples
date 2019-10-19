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

#include "published_socket_connector.h"
#include "socket_helper.h"

#include <sys/stat.h>

namespace kydroid {
namespace network {
PublishedSocketConnector::PublishedSocketConnector(
    const std::string& socket_file, const std::shared_ptr<Runtime>& rt,
    const std::shared_ptr<ConnectionCreator<boost::asio::local::stream_protocol>>& connection_creator)
    : socket_file_(remove_socket_if_stale(socket_file)),
      runtime_(rt),
      connection_creator_(connection_creator),
      acceptor_(rt->service(), socket_file_)//全局只有一个io_service和一个acceptor_(rt->service() is boost::asio::io_service)
{
    //if (QFile::exists(QString::fromStdString(socket_file_))) {
    //    ::chmod(socket_file_.c_str(), 0666);
    //}

    start_accept();//启动对 Unix 域 Socket 的监听:以 boost套接字的方式循环监控读取数据
}

PublishedSocketConnector::~PublishedSocketConnector() noexcept
{
//    qDebug() << "PublishedSocketConnector::~PublishedSocketConnector......";
}

/*
start_accept() 首先创建一个 boost::asio::local::stream_protocol::socket 对象用于保存下次接受的新 Socket 的相关信息，然后将这个对象，及作为处理新连接到来时的回调函数而创建的 lambda 表达式传递给 boost::asio::local::stream_protocol::acceptor 的 async_accept() 函数以启动对 Unix 域 Socket 的监听。
接收到新到来的连接之后，传给 boost::asio::local::stream_protocol::acceptor 的 async_accept() 的 lambda 回调函数直接将 Socket 交给 anbox::network::PublishedSocketConnector::on_new_connection() 来处理，anbox::network::PublishedSocketConnector::on_new_connection() 又将新连接交给 ConnectionCreator<boost::asio::local::stream_protocol> 来处理。
在 Anbox 的设计中，监听并接受连接与对新连接的处理，由两个完全不同的组件来处理，前者(监听并接受连接)由 anbox::network::Connector 处理，后者(对新连接的处理)由 anbox::network::ConnectionCreator 处理。anbox::network::ConnectionCreator 定义（位于 anbox/src/anbox/network/connection_creator.h）


*/
void PublishedSocketConnector::start_accept() {
    auto socket = std::make_shared<boost::asio::local::stream_protocol::socket>(runtime_->service());
    acceptor_.async_accept(*socket,
                         [this, socket](boost::system::error_code const& err) {
                           //qDebug() << "PublishedSocketConnector::start_accept async_accept";
                           on_new_connection(socket, err);
                         });

    //async_read  异步读客户端发来的消息             //async_开头的函数都是boost的异步方式
    //server:   async_accept              client:  async_connect异步连接
    //kobe test
    //sock_ptr sock(new socket_type(m_io));
    //m_accacceptor_eptor.async_accept(*sock, boost::bind(&this_type::accept_handler, this, boost::asio::placeholders::error, sock));
}

void PublishedSocketConnector::on_new_connection(std::shared_ptr<boost::asio::local::stream_protocol::socket> const& socket, boost::system::error_code const& err) {
    if (!err) {
        connection_creator_->create_connection_for(socket);//读取客户端发送过来的数据
    }
    if (err.value() == boost::asio::error::operation_aborted) {
        return;
    }

    //if (err.value() == boost::system::errc::success) {
        //qDebug() << "new boost connection success!!!";
    //}

    start_accept();//kobe:发送完毕后继续监听，否则io_service将认为没有事件处理而结束运行(每连接上一个socket都会调用  )
}

}  // namespace network
}  // namespace kydroid
