#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include <stdint.h>

class Thread {
public:
    // Public constructor.
    Thread();

    // Virtual destructor.
    virtual ~Thread();

    // Override this method in your own thread sub-classes. This will
    // be called when start() is invoked on the Thread instance.
    virtual intptr_t main() = 0;

    // Start a thread instance. Return true on success, false otherwise
    // (e.g. if the thread was already started or terminated).
    bool start();

    // Wait for thread termination and retrieve exist status into
    // |*exitStatus|. Return true on success, false otherwise.
    // NOTE: |exitStatus| can be NULL.
    bool  wait(intptr_t *exitStatus);

    // Check whether a thread has terminated. On success, return true
    // and sets |*exitStatus|. On failure, return false.
    // NOTE: |exitStatus| can be NULL.
    bool tryWait(intptr_t *exitStatus);

private:
    static void* thread_main(void* arg);

    pthread_t mThread;
    pthread_mutex_t mLock;
    bool mJoined;
    intptr_t mExitStatus;
    bool mIsRunning;
};

#endif  // _THREAD_H

