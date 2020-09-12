#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include <windows.h>

class Semaphore
{
private:
    HANDLE hSem;
public:
    Semaphore(unsigned int initialCount)
    {
        hSem = CreateSemaphore(NULL, initialCount, 0xFFFFFF, NULL);
    }
    ~Semaphore()
    {
        CloseHandle(hSem);
    }

    bool wait(unsigned int msec)
    {
        if(msec == INFINITE)
            msec--;
        return WaitForSingleObject(hSem, msec) == WAIT_OBJECT_0;
    }
};
#endif

