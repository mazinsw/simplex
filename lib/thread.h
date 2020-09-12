#ifndef _THREAD_H_
#define _THREAD_H_
#include <windows.h>

class Thread
{
private:
    HANDLE hThread;

    static DWORD WINAPI threadProc(LPVOID param)
    {
		reinterpret_cast<Thread*>(param)->run();
		return 0;
    }
protected:
    virtual void run() = 0;
public:
    Thread()
    {
        hThread = 0;
    }
    ~Thread()
    {
        CloseHandle(hThread);
    }

    bool start()
    {
        hThread = CreateThread(NULL, 0, &Thread::threadProc,
                               reinterpret_cast<void*>(this), 0, NULL);
        return hThread != 0;
    }

    void join()
    {
        WaitForSingleObject(hThread, INFINITE);
    }
    
    void Sleep(unsigned int msec = 0)
    {
        ::Sleep(msec);
    }
};
#endif

