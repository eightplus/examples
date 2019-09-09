#ifndef _WORKER_THREAD_H
#define _WORKER_THREAD_H

#include "thread.h"

class WorkerThread : public Thread {
public:
    WorkerThread(); 
    ~WorkerThread();

    static WorkerThread* create();

    bool isFinished() { return tryWait(NULL); }

private:
    virtual intptr_t main();
};

#endif
