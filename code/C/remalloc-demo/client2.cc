//g++ client2.cc AppSendInfo.pb.cc -o client2 -lprotobuf

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

using namespace std;

static void* do_event_loop_thread(void *arg)
{
    int fd = *(int *)arg;
    //开始循环，接收用户输入选择，并响应回复信息
    char buff[256];

    /*int rc = fscanf(stdin, "%s", buff);
    if (rc <= 0) {
        printf("null\n");
        return (void*)0;
    }
    if (!strcmp(buff, "q")) {
        exit(0);
    }*/

    while (fgets(buff, sizeof(buff), stdin) != NULL)
    {
        int iCmdType = 0;
        if(sscanf(buff, "%d", &iCmdType) == -1) {
            fprintf(stderr, "Input error: %s(errno: %d) And please input again\n", strerror(errno), errno);
            continue;
        }
        int op = 0;

        switch(iCmdType)
        {
            case 0:
                printf("request for thread to exit!\n");
                op = 1;
                break;
            case 1:
                break;
            default:
                printf("unrecognized command!\n");
                break;
        }

        if(op == 1)
            break;
    }

    return (void*)0;
}

int main(int argc, char ** argv)
{
    int fd;
    ssize_t read_num = -1;
    int read_len = 1;
    int read_sum = 0;
    char *tmp = NULL;
    char *recv_buf = NULL;

    const char *socket_file = "./unix_app";

    if (argc != 2) {
        printf("Usage: %s \"COMMAND\",  example: ./client single or ./client lists\n", argv[0]);
        exit(0);
    }

    if (strcmp(argv[1], "single") && strcmp(argv[1], "lists")) {
        printf("arguments error\n");
        return -1;
    }

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        printf("socket error: %s, %d\n", strerror(errno), errno);
        return -1;
    }

    size_t len = 0;
    len = strlen(socket_file);
    struct sockaddr_un address;
    memset(&address, 0, sizeof(struct sockaddr_un));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_file, len);
    len += offsetof(struct sockaddr_un, sun_path);

    if(connect(fd, (struct sockaddr*)&address, len) < 0) {
        printf("connect error: %s, %d\n", strerror(errno), errno);
        return -1;
    }

    //首先发送指令给客户端
    ssize_t stat = send(fd, argv[1], strlen(argv[1]), MSG_NOSIGNAL);
    if (stat < 0) {
        printf("send error: %s, %d\n", strerror(errno), errno);
        return -1;
    }
    //接着从服务端接收序列化的数据
    /*int numbytes;
    char buf[BUFFSIZE];
    numbytes = recv(fd, buf, BUFFSIZE, 0);
    buf[numbytes] = '\0';*/
    //以下代码首先molloc申请大小为BUFFSIZE的空间，当空间使用完了，且数据还没接收完的情况下，继续使用realloc动态申请空间
    tmp = (char *)malloc(BUFFSIZE);
    if (tmp == NULL) {
        printf("malloc failed!\n");
        close(fd);
        return -1;
    }
    memset(tmp, 0, BUFFSIZE);
    read_num = recv(fd, tmp, BUFFSIZE, 0);
    while (read_num == BUFFSIZE) {
        read_sum += read_num;
        read_len ++;
        recv_buf = (char *)realloc(tmp, read_len*BUFFSIZE);
        if (recv_buf == NULL) {
            printf("realloc failed!\n");
            free(tmp);
            close(fd);
            return -1;
        }
        printf("------------again\n");
        tmp = recv_buf;
        memset(tmp + (read_len - 1) * BUFFSIZE, 0, BUFFSIZE);
        read_num = recv(fd, tmp + (read_len - 1)*BUFFSIZE, BUFFSIZE, 0);
    }
    if (read_num > 0) {
        read_sum += read_num;
    }

    string str(tmp, read_sum);
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
    free(tmp);
    tmp = NULL;
    recv_buf = NULL;

    /*int err;
    pthread_t tid;
    pthread_create(&tid, NULL, do_event_loop_thread, (void *)&fd);
    if (err == 0) {
        while (1) {
            sleep(1);
        }
    }
    else {
        printf("pthread_create failed\n");
    }*/

    close(fd);
    return 0;
}
