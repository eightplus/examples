//g++ client1.cc AppSendInfo.pb.cc -o client1 -lprotobuf

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
#include <pthread.h>
#include <stdint.h>
#include "AppSendInfo.pb.h"

#define BUFFSIZE 128
#define MAX_TRIES 5

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

int main(int argc, char ** argv)
{
    int fd;
    char buffer[BUFFSIZE];
    char header[5];
    int  size = 0;
    const char *socket_file = "./unix_app";

    if (argc != 2) {
        printf("Usage: %s \"COMMAND\",  example: ./client single or ./client lists\n", argv[0]);
        exit(0);
    }

    if (strcmp(argv[1], "single") && strcmp(argv[1], "lists")) {
        printf("arguments error\n");
        return -1;
    }

    struct sockaddr_un socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, socket_file, sizeof(socket_address.sun_path));

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("socket error: %s, %d\n", strerror(errno), errno);
        return -1;
    }

    /* try to connect to server */
    {
        int  tries = MAX_TRIES;

        while (1) {
            if (connect(fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) >= 0) {
                break;
            }

            if (--tries <= 0) {
                printf("Could not connect after too many tries. Aborting (%s, %d)\n",  strerror(errno), errno);
                return 1;
            }
            printf("waiting 1s to wait for server.\n");
            sleep(1);
        }
    }


    size = my_channel_send(fd, argv[1], -1);//my_channel_send(fd, argv[1], strlen(argv[1]));
    if (size < 0) {
        printf("could not query sensor list: %s\n", strerror(errno));
        if (fd >= 0) {
            close(fd);
        }
        return size;
    }

    //读数据头，即数据的大小
    if (my_fd_read(fd, header, 4) != 4) {
        printf("can't read header: %s\n", strerror(errno));
        return -1;
    }
    header[4] = 0;

    size = 0;
    if (sscanf(header, "%04x", &size) != 1) {
        printf("malformed header: '%.*s'\n", 4, header);
        return -1;
    }

    //读数据： 得到数据头(即数据的大小)后，再申请该大小的空间去存放数据
    char buffer2[size];
    if (my_fd_read(fd, buffer2, size) != size) {
        printf("can't read data: %s\n", strerror(errno));
        return -1;
    }

    string str(buffer2, size);
    if (!strcmp(argv[1], "single")) {
        com::eightplus::appinfo::Notification p;
        p.ParseFromString(str);
        cout << "package_name: " << p.package_name() << endl;
        cout << "app_name: " << p.app_name() << endl;
        cout << "text: " << p.text() << endl;
    }
    else if (!strcmp(argv[1], "lists")) {
        com::eightplus::appinfo::RunningAppList event;
        event.ParseFromString(str);
        for (int n = 0; n < event.item_size(); n++) {
            com::eightplus::appinfo::RunningAppItem app = event.item(n);
            printf("app_name:%s, package_name:%s\n", app.app_name().c_str(), app.app_name().c_str());
        }
    }
    else {
        printf("oh,no! %s\n", str.c_str());
    }

    /*
    //读数据头和数据，当读取到的数据头的大小大于buffer申请的空间大小后，不再读取数据
    int ret = my_channel_recv(fd, buffer, sizeof buffer-1);//my_channel_recv(fd, buffer, sizeof(buffer) - 1U);
    if (ret < 0 || ret > BUFFSIZE-1 || buffer[0] == '\0') {
        printf("could not receive sensor list: %s\n",  strerror(errno));
        if (fd >= 0) {
            close(fd);
        }
        return ret;
    }
    buffer[ret] = 0;//buffer[ret] = '\0';

    string str(buffer, ret);
    if (!strcmp(argv[1], "single")) {
        com::eightplus::appinfo::Notification p;
        p.ParseFromString(str);
        cout << "package_name: " << p.package_name() << endl;
        cout << "app_name: " << p.app_name() << endl;
        cout << "text: " << p.text() << endl;
    }
    else if (!strcmp(argv[1], "lists")) {
        com::eightplus::appinfo::RunningAppList event;
        event.ParseFromString(str);
        for (int n = 0; n < event.item_size(); n++) {
            com::eightplus::appinfo::RunningAppItem app = event.item(n);
            printf("app_name:%s, package_name:%s\n", app.app_name().c_str(), app.app_name().c_str());
        }
    }
    else {
        printf("oh,no! %s\n", str.c_str());
    }
    */

    return 0;
}
