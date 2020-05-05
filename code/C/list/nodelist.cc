/*
 * Author: lixiang <kobe24_lixang@126.com>
 *
 * 单项链表的创建和释放
 *
 * g++ nodelist.cc -o nodelist -g
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#define MAX_LEN 256

typedef struct _SYSTEM_NODE
{
    char cpu[MAX_LEN];
    char memory[MAX_LEN];
} SYSTEM_NODE;

typedef struct _USER_NODE
{
    char name[MAX_LEN];
    char address[MAX_LEN];
} USER_NODE;

typedef struct _MY_NODE
{
    struct _MY_NODE *Next;
    union {
        USER_NODE m_user;
        SYSTEM_NODE m_system;
    } Node;
} MyNode;

MyNode *m_globalNode=NULL;
int gCount = 0;

//打印链表
void print_node_list()
{
    MyNode *node = m_globalNode;
    int i = 0;
    while (node != NULL) {
        printf("### %d (Address=0x%x, Key=%s, Value=%s)\n" , i++ , (void*)node, node->Node.m_user.name, node->Node.m_user.address);
        node = node->Next;
    }
}

//获取链表头
MyNode **get_node_list_head()
{
    printf("my return = 0x%x , source return = 0x%x\n" , ((void*)(&m_globalNode)) , ((void*)m_globalNode));
    return &m_globalNode;
}

//获取下一个节点
MyNode *get_next_node(MyNode *node)
{
    return node->Next;
}

//获取指定序号的记录
void *get_node_by_index(int index)
{
    int count = 0;
    if (index < 0 || index > gCount) {
        return NULL;
    }

    MyNode *node = m_globalNode;
    while (node != NULL) {
        if (index == count) {
            return node;
        }
        else {
            node = node->Next;
            count++;
        }
    }

    return NULL;
}

int new_node_list()
{
    gCount = 0;

    for (int i=0; i<5; i++) {
        MyNode *node = new MyNode;
        if (node == NULL) {
            return -1;
        }
        memset(node, 0, sizeof(MyNode));

        USER_NODE *newNode = &(node->Node.m_user);
        strcpy(newNode->address, "hunan changsha");
        strcpy(newNode->name,"lixiang");

        node->Next = m_globalNode;
        m_globalNode = node;
        gCount ++;
    }
    printf("AAA %d : Current List:\n" , __LINE__);
    print_node_list();

    for(int i=0; i<5; i++) {
        MyNode *node = new MyNode;
        if (node == NULL) {
            return -1;
        }
        memset(node, 0, sizeof(MyNode));

        USER_NODE *newNode = &(node->Node.m_user);
        strcpy(newNode->address, "hunan yueyang");
        strcpy(newNode->name,"kobe");

        node->Next = m_globalNode;
        m_globalNode = node;
        gCount ++;
    }
    printf("BBB %d : Current List:\n" , __LINE__);
    print_node_list();

    return gCount;
}

void create_node()
{
    int result = new_node_list();
    if (result > 0) {
        USER_NODE *tmp_user_node;
        MyNode *currentNode = *(get_node_list_head());
        if (currentNode != NULL) {
            tmp_user_node = &(currentNode->Node.m_user);
            printf("lixiang test index=1, tmp_user_node->name=%s, tmp_user_node->address=%s\n", tmp_user_node->name, tmp_user_node->address);
            for(int i=2; i<result;i ++) {
                //method 1:
                void *resultNode = get_node_by_index(i);
                MyNode *p = (MyNode *)resultNode;
                if (p == NULL) {
                    continue;
                }
                tmp_user_node = NULL;
                tmp_user_node = &(p->Node.m_user);
                if (tmp_user_node == NULL)
                    continue;
                printf("kobe test http index=%d, tmp_user_node->name=%s, tmp_user_node->address=%s\n", i, tmp_user_node->name, tmp_user_node->address);


                //method 2:
                /*MyNode *nextNode = get_next_node(currentNode);
                if (nextNode == NULL) {
                    continue;
                }
                tmp_user_node = NULL;
                tmp_user_node = &(nextNode->Node.m_user);
                if (tmp_user_node != NULL)
                    printf("kobe test http index=%d, tmp_user_node->name=%s, tmp_user_node->address=%s\n", i, tmp_user_node->name, tmp_user_node->address);
                currentNode = NULL;
                currentNode = nextNode;*/
            }
        }
    }
}

//删除整个单向链表，释放内存
void free_node_list(MyNode **head)
{
    int i = 0;
    MyNode *node = NULL;

    while (*head != NULL) {
	printf("---------------------------第%d次--------------------------\n" , ++ i);
        printf("当前t_head指向：0x%x, 当前g_head指向：0x%x\n" , (void*)*head , (void*)m_globalNode);
        print_node_list();
	printf("-----------------------------------------------------------\n");
        node = (*head)->Next;
        free(*head);
        (*head) = node;
    }
}

void free_node()
{
    MyNode **node = get_node_list_head();
    free_node_list(node);
}

int main(int argc, char* argv[])
{
    printf("command ready:\n0,退出程序\n1,创建链表\n2,释放链表\n");
    char buff[256];
    while (fgets(buff, sizeof(buff), stdin) != NULL)
    {
        int iCmdType = 0;
        if (sscanf(buff, "%d", &iCmdType) == -1) {//command error
            fprintf(stderr, "Input eroor: %s(errno: %d) And please input again\n", strerror(errno), errno);
            continue;
        }
        int op = 0;
        switch (iCmdType) {
            case 0:
                printf("exit!\n");
                op = 1;
                break;
            case 1:
                printf("new!\n");
                create_node();
                break;
            case 2:
                printf("free!\n");
                free_node();
                break;
            default:
                break;
        }
        //exit
        if(op == 1) {
            break;
        }
    }

    return 0;
}
