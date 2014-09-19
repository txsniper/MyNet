#ifndef _BASE_THREAD_OPT_
#define _BASE_THREAD_OPT_

namespace MyNet
{
    namespace ThreadOpt
    {
        extern __thread int t_cachedtid;
        extern __thread const char* t_threadname;

        void cacheTid();
        int tid()
        {
            if(t_cachedtid == 0)
                cacheTid();
            else
                return t_cachedtid;
        }

        const char* threadName()
        {
            return t_threadname;
        }

    }
}
#endif
