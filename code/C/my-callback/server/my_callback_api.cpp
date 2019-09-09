#include "my_callback_api.h"
#include "WorkerThread.h"

#include <string.h>
#include <stdio.h>

static WorkerThread *m_workerThread = NULL;

int initCallbackDaemon(const char* user)
{
    printf("client user:%s\n", user);
    if (m_workerThread) {
        return false;
    }

    m_workerThread = WorkerThread::create();
    if (!m_workerThread) {
        fprintf(stderr, "Failed to create WorkerThread\n");
        return false;
    }
    else if (!m_workerThread->start()) {
        fprintf(stderr,"Failed to start WorkerThread\n");
        delete m_workerThread;
        m_workerThread = NULL;
        return false;
    }

    return true;
}

int destroyCallbackDaemon(void)
{
    bool ret = false;
    if (m_workerThread) {
        // wait for the thread to exit
        ret = m_workerThread->wait(NULL);

        delete m_workerThread;
        m_workerThread = NULL;
    }

    return ret;
}
