#include "thread.h"

#include <assert.h>
#include <stdio.h>

namespace {

class ScopedLocker {
public:
    ScopedLocker(pthread_mutex_t* mutex) : mMutex(mutex) {
        pthread_mutex_lock(mMutex);
    }

    ~ScopedLocker() {
        pthread_mutex_unlock(mMutex);
    }
private:
    pthread_mutex_t* mMutex;
};

}  // namespace

Thread::Thread() :
    mThread((pthread_t)NULL),
    mLock(),
    mJoined(false),
    mExitStatus(0),
    mIsRunning(false) {
    pthread_mutex_init(&mLock, NULL);
}

Thread::~Thread() {
    //assert(!mIsRunning);
    //assert(mJoined);
    pthread_mutex_destroy(&mLock);
}

bool Thread::start() {
    bool ret = true;
    pthread_mutex_lock(&mLock);
    mIsRunning = true;
    if (pthread_create(&mThread, NULL, thread_main, this)) {
        ret = false;
        mIsRunning = false;
    }
    pthread_mutex_unlock(&mLock);
    return ret;
}

bool Thread::wait(intptr_t *exitStatus) {
    {
        ScopedLocker locker(&mLock);
        if (!mIsRunning) {
            // Thread already stopped.
            if (exitStatus) {
                *exitStatus = mExitStatus;
            }
            if (!mJoined) {
                // reclaim thread stack
                pthread_join(mThread, NULL);
                mJoined = true;
            }
            return true;
        }
    }

    // NOTE: Do not hold the lock when waiting for the thread to ensure
    // it can update mIsRunning and mExitStatus properly in thread_main
    // without blocking.
    void *retval;
    if (pthread_join(mThread, &retval)) {
        return false;
    }
    if (exitStatus) {
        *exitStatus = (intptr_t)retval;
    }
    // Note: Updating mJoined must be performed inside the lock to avoid
    //       race conditions between two threads waiting for the same thread
    //       that just completed its execution.
    {
        ScopedLocker locker(&mLock);
        mJoined = true;
    }
    return true;
}

bool Thread::tryWait(intptr_t *exitStatus) {
    ScopedLocker locker(&mLock);
    if (mIsRunning) {
        return false;
    }
    if (!mJoined) {
        // Reclaim stack.
        pthread_join(mThread, NULL);
        mJoined = true;
    }
    if (exitStatus) {
        *exitStatus = mExitStatus;
    }
    return true;
}

// static
void* Thread::thread_main(void *arg) {
    Thread* self = reinterpret_cast<Thread*>(arg);
    intptr_t ret = self->main();

    pthread_mutex_lock(&self->mLock);
    self->mIsRunning = false;
    self->mExitStatus = ret;
    pthread_mutex_unlock(&self->mLock);

    return (void*)ret;
}
