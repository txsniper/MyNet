#ifndef _BASE_THREAD_OPT_
#define _BASE_THREAD_OPT_

#include <sys/types.h>
#include <unistd.h>
namespace MyNet
{
    namespace ThreadOp
    {
        extern __thread int t_cachedtid;
        extern __thread const char* t_threadname;
        pid_t gettid();
        void cacheTid();
        int tid()
        {
            if(t_cachedtid == 0)
                cacheTid();
            return t_cachedtid;
        }

        const char* threadName()
        {
            return t_threadname;
        }

    }
}
#endif
