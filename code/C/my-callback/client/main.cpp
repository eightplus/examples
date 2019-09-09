#include <stdio.h>
#include <unistd.h>

#include "client.h"
#include "../server/DataController.h"

static int32_t postData1(uint32_t id, int32_t age)
{
    printf("postData1 received\n");
    printf("id: %d\n", id);
    printf("age: %d\n", age);

    return 0;
}

static void postData2(const char *name, uint32_t id, int32_t age)
{
    printf("postData2 received\n");
    printf("name: %s\n", name);
    printf("id: %d\n", id);
    printf("age: %d\n", age);
}

static unsigned char *getStrData()
{
    printf("getStrData received\n");
    return (unsigned char *)"callback-string-test";
}

static uint32_t getIntData()
{
    printf("getIntData received\n");
    return 1024;
}

int main(int argc, char** argv)
{
    DataController *m_dataController;
    
    if (checkCallbackLibrary() != 0) {
        fprintf(stderr, "Failed to initialize callback library\n");
        return -1;
    }

    if (startCallbackDaemon() != 0) {
        fprintf(stderr, "Failed to start callback library\n");
        return -1;
    }

    m_dataController = DataController::getInstance();
    if (m_dataController) {
        m_dataController->postData1 = postData1;
        m_dataController->postData2 = postData2;
        m_dataController->getStrData = getStrData;
        m_dataController->getIntData = getIntData;
    }
    else
    	printf("new DataController object failed\n");

    while(1)
        sleep(1);

    stopCallbackDaemon();

    return 0;

}
