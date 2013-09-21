#include "ext_thread.h"

namespace ext {

    int MutexInit(MutexType *mutex)
    {
#ifdef WIN32
        *mutex = CreateMutex(NULL, FALSE, NULL);
        return *mutex != NULL ? 0 : -1;
#else
        return pthread_mutex_init(mutex, NULL);
#endif
        return -1;
    }

    int MutexLock(MutexType *mutex)
    {
#ifdef WIN32
        return (WaitForSingleObject(*mutex, INFINITE) != WAIT_FAILED ? 0 : -1);
#else
        return pthread_mutex_lock(mutex);
#endif
        return -1;
    }

    int MutexUnlock(MutexType *mutex)
    {
#ifdef WIN32
        return ReleaseMutex(*mutex) ? 0 : -1 ;
#else
        return pthread_mutex_unlock( mutex );
#endif
        return -1;
    }

    int MutexDestroy(MutexType *mutex)
    {
#ifdef WIN32
        return CloseHandle(*mutex) ? 0 : -1;
#else
        return pthread_mutex_destroy(mutex);
#endif
    }

    static void ThreadCallback(Thread* who) {
#ifndef WIN32
        // Далаем поток "убиваемым" через pthread_cancel.
        int old_thread_type;
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_thread_type);
#endif
        who->Execute(); 
    }

#ifdef WIN32

    Thread::~Thread() {
        CloseHandle(__handle);
    }

    void Thread::Start() {
        __handle = CreateThread(
            0, 0, 
            reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadCallback), this,
            0, 0
            );
    }

    void Thread::Join() {
        WaitForSingleObject(__handle,  INFINITE);
    }

    void Thread::Kill() {
        TerminateThread(__handle, 0);
    }

#else

    Thread::~Thread() {
    }

    extern "C"
        typedef void *(*pthread_callback)(void *);

    void Thread::Start() {
        pthread_create(
            &__handle, 0, 
            reinterpret_cast<pthread_callback>(ThreadCallback), 
            this
            );
    }

    void Thread::Join() {
        pthread_join(__handle, 0);
    }

    void Thread::Kill() {
        pthread_cancel(__handle);
    }

#endif

} // ext
