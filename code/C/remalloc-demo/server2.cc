//g++ server2.cc AppSendInfo.pb.cc -o server2 -lprotobuf

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
  
int main()  
{  
    int listenfd, connectfd, numbytes;
    connectfd = -1;
    char buf[BUFFSIZE];
    const char *socket_file = "./unix_app";
    struct sockaddr_un srv_addr;

    ssize_t read_num = -1;
    int read_len = 1, read_sum = 0;
    char *tmp = NULL;
    char *recv_buf = NULL;

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

    /*int len = offsetof(struct sockaddr_un, sun_path) + strlen(srv_addr.sun_path);
    if(bind(listenfd, (struct sockaddr *)&srv_addr, len) < 0) {
        printf("bind error\n");
        close(listenfd);
        unlink(socket_file);
	return -1;
    }*/
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

        //首先接收客户端发送的指令
        /*numbytes = recv(connectfd, buf, BUFFSIZE, 0);
        buf[numbytes] = '\0';
        string msg = buf;*/

        //以下代码首先molloc申请大小为BUFFSIZE的空间，当空间使用完了，且数据还没接收完的情况下，继续使用realloc动态申请空间
        read_len = 1;
        read_sum = 0;
        //read and printf sent client info
        tmp = (char*) malloc(BUFFSIZE);
        if (tmp == NULL) {
            printf("cannot malloc buffer\n");
            close(listenfd);
            unlink(socket_file);
            return 1;
        }

        memset(tmp, 0, BUFFSIZE);
        read_num = recv(connectfd, tmp, BUFFSIZE, 0);
        while (read_num == BUFFSIZE) {
            read_sum += read_num;
            read_len++;
            recv_buf = (char*) realloc(tmp, read_len * BUFFSIZE);
            if (recv_buf == NULL) {
                printf("cannot realloc buffer\n");
                free(tmp);
                close(listenfd);
                unlink(socket_file);
                return 1;
            }

            tmp = recv_buf;
            memset(tmp + (read_len - 1) * BUFFSIZE, 0, BUFFSIZE);
            read_num = recv(connectfd, tmp + (read_len - 1) * BUFFSIZE, BUFFSIZE, 0);
        }
        if (read_num > 0) {
            read_sum += read_num;
        }

        string msg = string(tmp, read_sum);
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
            send(connectfd, bts, sizeof(bts), 0);
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
            send(connectfd, bts, sizeof(bts), 0);
        }
        else {
            printf("WTF\n");
            send(connectfd, "WTF!\n", 6, 0);
        }
        free(tmp);
        tmp = NULL;
        recv_buf = NULL;

        close(connectfd);
    }
  
    close(listenfd);

    return 0;
}
