#ifndef _BASE_THREAD_H_
#define _BASE_THREAD_H_

#include <string.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
namespace MyNet
{
    namespace base
    {
        class Thread
        {
            public:
                typedef boost::function<void()> ThreadFunc;
                Thread(const ThreadFunc& func, const std::string& name);
                ~Thread();
                void start();
                int join();
            private:
                static void runInThread(void *thread_obj);
            private:
                pthread_t m_threadId;   // pthread id, used in pthread functions
                pid_t m_tid;            // thread id, used by linux system ,it's different from pthread id
                std::string m_name;
                ThreadFunc m_func;
                bool m_joined;          // Wether the thread has joined or not
                bool m_started;         // Wether the thread has started or not
        };
    }
}
#endif
