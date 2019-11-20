//g++ server1.cc AppSendInfo.pb.cc -o server1 -lprotobuf

#include <iostream>  
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "AppSendInfo.pb.h"
   
#define BUFFSIZE 128
#define NUM 10
  
using namespace std;

// https://www.androidos.net.cn/android/7.1.1_r28/xref/hardware/libhardware/include/hardware/qemud.h
static int my_fd_write(int fd, const void* buff, int len)
{
    int  len2;
    do {
        len2 = write(fd, buff, len);
    } while (len2 < 0 && errno == EINTR);

    return len2;
}

static int my_fd_read(int fd, void* buff, int len)
{
    int  len2;
    do {
        len2 = read(fd, buff, len);
    } while (len2 < 0 && errno == EINTR);

    return len2;
}

static int my_channel_send(int  fd, const void*  msg, int  msglen)
{
    char  header[5];

    if (msglen < 0)
        msglen = strlen((const char*)msg);

    if (msglen == 0)
        return 0;

    snprintf(header, sizeof header, "%04x", msglen);
    if (my_fd_write(fd, header, 4) != 4) {
        printf("can't write header: %s\n", strerror(errno));
        return -1;
    }

    if (my_fd_write(fd, msg, msglen) != msglen) {
        printf("can't write data: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

//当头的长度大于msgsize，不再继续读取数据段，其中msgsize为msg申请的最大长度
static int my_channel_recv(int  fd, void*  msg, int  msgsize)
{
//    char header[5] = {0};
//    int len = read(fd, header, 4);
//    if (len != 4/* || header[0] != 'O'*/) {
//        return -1;
//    }
//    int32_t lens = atoi(header);
//    if (lens > msgsize)
//        return -1;

    char  header[5];
    int   size;

    if (my_fd_read(fd, header, 4) != 4) {
        printf("can't read header: %s\n", strerror(errno));
        return -1;
    }
    header[4] = 0;
    if (sscanf(header, "%04x", &size) != 1) {
        printf("malformed header: '%.*s'\n", 4, header);
        return -1;
    }
    if (size > msgsize)
        return -1;

    if (my_fd_read(fd, msg, size) != size) {
        printf("can't read data: %s\n", strerror(errno));
        return -1;
    }
    return size;
}

int main()  
{  
    int listenfd, connectfd;
    connectfd = -1;
    const char *socket_file = "./unix_app";
    struct sockaddr_un srv_addr;

    if((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("socket error\n");
        return -1;
    }

    memset(&srv_addr, 0, sizeof(srv_addr));//memset(&srv_addr, 0, sizeof(struct sockaddr_un));
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, socket_file, sizeof(srv_addr.sun_path) - 1);

    if (access(srv_addr.sun_path, F_OK) == 0) {
        unlink(srv_addr.sun_path);
    }

    int ret = bind(listenfd, (struct sockaddr*) &srv_addr, sizeof(srv_addr));
    if (ret == -1) {
        printf("bind error\n");
        close(listenfd);
        unlink(socket_file);
        return 1;
    }

    chmod(srv_addr.sun_path, S_IRWXU | S_IRWXG | S_IRWXO);
    if (listen(listenfd, NUM) < 0) {
        printf("listen error\n");
        close(listenfd);
        unlink(socket_file);
	return -1;
    }

    socklen_t sin_size;
  
    while(1){
        struct sockaddr_un addr;
        sin_size = sizeof(addr);
        connectfd = accept(listenfd, (struct sockaddr *)&addr, (socklen_t*)&sin_size);
        if (connectfd < 0 && errno == EINTR) {
            continue;
        }

        // test code
        /*
        uint32_t client_info = 3;
        if (write(connectfd, &client_info, sizeof(client_info)) < 0) {
            return -1;
        }
        auto bytes_read = read(connectfd, &client_info, sizeof(client_info));
        if (bytes_read < 0)
        {
            close(connectfd);
            connectfd = -1;
            return -EIO;
        }
*/

        char header[5];
        int  size = 0;
        //读数据头，即数据的大小
        if (my_fd_read(connectfd, header, 4) != 4) {
            printf("can't read header: %s\n", strerror(errno));
            close(listenfd);
            close(connectfd);
            unlink(socket_file);
            return -1;
        }
        header[4] = 0;
        if (sscanf(header, "%04x", &size) != 1) {
            printf("malformed header: '%.*s'\n", 4, header);
            close(listenfd);
            close(connectfd);
            unlink(socket_file);
            return -1;
        }

        //读数据： 得到数据头(即数据的大小)后，再申请该大小的空间去存放数据
        char buffer2[size];
        if (my_fd_read(connectfd, buffer2, size) != size) {
            printf("can't read data: %s\n", strerror(errno));
            close(listenfd);
            close(connectfd);
            unlink(socket_file);
            return -1;
        }
        string msg(buffer2, size);
        if (msg == "single") {
            //数据序列化
            string data;
            com::eightplus::appinfo::Notification p;
            p.set_package_name("tencent.news");
            p.set_app_name("news");
            p.set_text("news app");
            p.SerializeToString(&data);

            char bts[data.length()];
            strcpy(bts, data.c_str());
            printf("single\n");

            //接着发送序列化数据给客户端
            my_channel_send(connectfd, bts, sizeof(bts));
        }
        else if (msg == "lists") {
            //数据序列化
            string data;
            com::eightplus::appinfo::RunningAppList event;
            com::eightplus::appinfo::RunningAppItem *item;
            item = event.add_item();
            item->set_app_name("proto1");
            item->set_package_name("com.eightplus.proto1");

            item = event.add_item();
            item->set_app_name("proto2");
            item->set_package_name("com.eightplus.proto2");

            item = event.add_item();
            item->set_app_name("proto3");
            item->set_package_name("com.eightplus.proto3");

            item = event.add_item();
            item->set_app_name("proto4");
            item->set_package_name("com.eightplus.proto4");

            item = event.add_item();
            item->set_app_name("proto5");
            item->set_package_name("com.eightplus.proto5");

            item = event.add_item();
            item->set_app_name("proto6");
            item->set_package_name("com.eightplus.proto6");

            item = event.add_item();
            item->set_app_name("proto7");
            item->set_package_name("com.eightplus.proto7");

            item = event.add_item();
            item->set_app_name("proto8");
            item->set_package_name("com.eightplus.proto8");

            event.SerializeToString(&data);

            int size = event.item_size();
            printf("lists size:%d\n", size);

            char bts[data.length()];
            strcpy(bts, data.c_str());

            //接着发送序列化数据给客户端
            my_channel_send(connectfd, bts, sizeof(bts));
        }
        else {
            printf("WTF\n");
            my_channel_send(connectfd, "WTF!", -1);
        }

        //读数据头和数据，当读取到的数据头的大小大于buffer申请的空间大小后，不再读取数据
        /*
        int  ret = 0;
        char buffer[BUFFSIZE];
        ret = my_channel_recv(connectfd, buffer, sizeof buffer-1);//my_channel_recv(connectfd, buffer, sizeof(buffer) - 1U);
        if (ret < 0 || ret > BUFFSIZE-1 || buffer[0] == '\0') {
            printf("could not receive sensor list: %s\n",  strerror(errno));
            close(listenfd);
            close(connectfd);
            unlink(socket_file);
            return 1;
        }
        buffer[ret] = 0;//buffer[ret] = '\0';

        string msg = string(buffer, ret);
        if (msg == "single") {
            //数据序列化
            string data;
            com::eightplus::appinfo::Notification p;
            p.set_package_name("tencent.news");
            p.set_app_name("news");
            p.set_text("news app");
            p.SerializeToString(&data);

            char bts[data.length()];
            strcpy(bts, data.c_str());
            printf("single\n");

            //接着发送序列化数据给客户端
            my_channel_send(connectfd, bts, sizeof(bts));
        }
        else if (msg == "lists") {
            //数据序列化
            string data;
            com::eightplus::appinfo::RunningAppList event;
            com::eightplus::appinfo::RunningAppItem *item;
            item = event.add_item();
            item->set_app_name("proto1");
            item->set_package_name("com.eightplus.proto1");

            item = event.add_item();
            item->set_app_name("proto2");
            item->set_package_name("com.eightplus.proto2");

            item = event.add_item();
            item->set_app_name("proto3");
            item->set_package_name("com.eightplus.proto3");

            item = event.add_item();
            item->set_app_name("proto4");
            item->set_package_name("com.eightplus.proto4");

            item = event.add_item();
            item->set_app_name("proto5");
            item->set_package_name("com.eightplus.proto5");

            item = event.add_item();
            item->set_app_name("proto6");
            item->set_package_name("com.eightplus.proto6");

            item = event.add_item();
            item->set_app_name("proto7");
            item->set_package_name("com.eightplus.proto7");

            item = event.add_item();
            item->set_app_name("proto8");
            item->set_package_name("com.eightplus.proto8");

            event.SerializeToString(&data);

            int size = event.item_size();
            printf("lists size:%d\n", size);

            char bts[data.length()];
            strcpy(bts, data.c_str());

            //接着发送序列化数据给客户端
            my_channel_send(connectfd, bts, sizeof(bts));
        }
        else {
            printf("WTF\n");
            my_channel_send(connectfd, "WTF!", -1);
        }
        */
        close(connectfd);
    }
  
    close(listenfd);

    return 0;
}
