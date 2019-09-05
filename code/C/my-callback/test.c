/*
 * Copyright (C) 2018 ~ 2020 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * gcc -o test test.c -lpthread
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef struct _MyMsg {
    double percentage;
    char info[32];
} MyMsg;

//函数指针的格式为：void (*ptr)(int n) 即：返回值(指针名)(参数列表)
typedef void (*TEST1_CALLBACK)(int n);

//函数指针的格式为：int (*ptr)(char *p) 即：返回值(指针名)(参数列表)
typedef int (*TEST2_CALLBACK)(char *p);//为回调函数命名，类型命名为TEST2_CALLBACK，参数为char *p

//方法指针的格式为：void (*ptr)(MyMsg *msg) 即：返回值(指针名)(参数列表)
typedef void (*PROCESS_CALLBACK)(MyMsg *msg);

typedef struct _MyMeta {
    int value;
    TEST1_CALLBACK call_back;
} MyMeta;

//------------------------------start-------------------------------
//处理进度的回调函数
void processFunction(MyMsg *msg)
{
    if (strlen(msg->info) > 0)
        printf("processFunction:%s %f\n", msg->info, msg->percentage);
    else
        printf("processFunction:%f\n", msg->percentage);
}

PROCESS_CALLBACK m_processCallback = NULL;
//注册函数： 执行回调函数processFunction(用一个全局变量存放，在底层有需要的任何地方都可以调用)
void registerProcessCallbackFunc(PROCESS_CALLBACK func)
{
    m_processCallback = func;
}
//------------------------------end-------------------------------



//------------------------------start-------------------------------
// 打印字符串的回调函数
int test2Function(char *str)
{
    printf("test2Function: %s\n", str);
    return 0;
}

int test3Function(char *str)
{
    printf("test3Function: %s\n", str);
    return 0;
}

// 注册函数： 执行回调函数test2Function，方式一：通过命名方式，callback可以看做是TEST2_CALLBACK的别名
int registerTest2Func1(TEST2_CALLBACK callback, char *str)
{
    callback(str);
    return 0;
}
// 注册函数： 执行回调函数test3Function，方式二：直接通过方法指针
int registerTest2Func2(char *str, int (*ptr)())  //或者是int registerTest2Func2(char *str, int (*ptr)(char *))同时ptr可以任意取名
{
    (*ptr)(str);//执行test3Function
}
//------------------------------end-------------------------------




//------------------------------start-------------------------------
// 线程回调函数
void *thread_func(void *args)
{
    MyMeta *meta = (MyMeta *)args;

    int res = 100;

    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 50;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }

    // 此处用sleep来模拟底层在执行某项操作，sleep完成后，即得到相关资源后，然后再调用用户提供的程序
    sleep(3);

    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 80;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }
    // 执行回调函数
    meta->call_back(meta->value);
    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 90;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }
    // 执行回调函数
    meta->call_back(res);

    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 100;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }
}

// 注册函数：执行回调函数test2Function，这里注册函数开启一个线程，在线程中等待3s模拟在底层获取数据，3s后，执行回调函数test2Function，即实现异步操作
void registerUserFunc(TEST1_CALLBACK func, int value)
{
    pthread_t tid;
    MyMeta *meta = (MyMeta *)malloc(sizeof(MyMeta));

    meta->value = value;
    meta->call_back = func;

    pthread_create(&tid, NULL, thread_func, (void *)meta);
    pthread_detach(tid);
}

// 回调函数，用户写的该函数，但不用用户自己去调用，而是通过指针由底层的注册函数去调用
void test1Function(int value)
{
    printf("test1Function: value = %d\n", value);
}
//------------------------------end-------------------------------

int main(int argc, const char *argv[])
{
    int value = 24;

    //---------------------------------
    PROCESS_CALLBACK process_func = processFunction;
    registerProcessCallbackFunc(process_func);

    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 0;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }

    //-----------------------------------
    TEST2_CALLBACK test2_func = test2Function;
    char *str = "hello, world!";
    registerTest2Func1(test2_func, str);

    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 20;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }
    char *str2 = "hello, lixiang!";
    registerTest2Func2(str2, test3Function);
    if (m_processCallback!=NULL) {
        MyMsg msg;
        msg.percentage = 40;
        strcpy(msg.info, "lixiang\n");
        m_processCallback(&msg);
    }

    //-----------------------------------
    TEST1_CALLBACK test1_func = test1Function;
    registerUserFunc(test1_func, value);

    pause();

    return 0;
}
