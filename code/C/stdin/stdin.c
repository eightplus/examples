/*
gcc -o run stdin.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// Read message from stdin
void test_stdin_method1()
{
    char buf[256];
    printf("# please input your command!!!\n");
    while (fscanf(stdin, "%s", buf) > 0) {
        int op = 0;
        if (!strcmp(buf, "q")) {
            printf("Exit......\n");
            op = 1;
        }
        else {
            printf("input command `%s'\n", buf);
        }

        if(op == 1)
            break;
    }
}

// Read message from stdin
void test_stdin_method2()
{
    char buf[256];
    printf("# please input your command(number)!!!\n");
    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        int iCmdType = 0;
        if(sscanf(buf, "%d", &iCmdType) == -1) {
            fprintf(stderr, "Input error: %s(errno: %d) And please input again\n", strerror(errno), errno);
            continue;
        }
        int op = 0;

        switch(iCmdType)
        {
            case 0:
                printf("Exit......\n");
                op = 1;
                break;
            default:
                printf("Command Type : %d\n", iCmdType);
                break;
        }

        if(op == 1)
            break;
    }
}

int main(int argc, char ** argv)
{
    test_stdin_method1();
    //test_stdin_method2();

    return 0;
}
