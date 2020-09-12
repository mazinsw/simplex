#ifndef _MUTEX_H_
#define _MUTEX_H_
#include <windows.h>

class Mutex
{
private:
    CRITICAL_SECTION criticalSection;
public:
    Mutex()
    {
        InitializeCriticalSection(&criticalSection);
    }
    ~Mutex()
    {
        DeleteCriticalSection(&criticalSection);
    }

    void lock()
    {
        EnterCriticalSection(&criticalSection);
    }

    void unlock()
    {
        LeaveCriticalSection(&criticalSection);
    }
};
#endif

