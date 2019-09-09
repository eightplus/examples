#include "WorkerThread.h"
#include "DataController.h"

#include <unistd.h>

WorkerThread::WorkerThread() : Thread()
{

}

WorkerThread::~WorkerThread()
{

}

WorkerThread* WorkerThread::create()
{
    return new WorkerThread();
}


intptr_t WorkerThread::main()
{
    DataController *m_dataController;
    m_dataController = DataController::getInstance();
    if (!m_dataController) {
	return 1;
    }

    while (1) {
        bool progress;
        do {
            progress = false;

	    sleep(5);

            size_t last = m_dataController->parseData();
            if (last > 0) {
                progress = true;
            }
        } while(progress);

    }

    return 0;
}
