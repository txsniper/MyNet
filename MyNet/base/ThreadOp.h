#ifndef _BASE_THREAD_OPT_
#define _BASE_THREAD_OPT_

#include <sys/types.h>
#include <unistd.h>
namespace MyNet
{
namespace ThreadOp
{
    extern __thread int t_cachedtid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadname;
    pid_t gettid();
    void cacheTid();
    inline int tid()
    {
        if(__builtin_expect(t_cachedtid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedtid;
    }
    inline const char* threadName()
    {
        return t_threadname;
    }

    inline const char* tidString()
    {
        return t_tidString;
    }

    inline int tidStringLength()
    {
        return t_tidStringLength;
    }

    bool isMainThread();

    void sleepUsec(int64_t usec);

}
}
#endif
