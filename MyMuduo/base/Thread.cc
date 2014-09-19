#include "Thread.h"
#include "ThreadOpt.h"
#include <pthread.h>
#include <assert.h>
namespace MyNet
{
    namespace ThreadOp
    {
        __thread int t_cachedtid = 0;
        __thread const char* t_threadname = "default";

        pid_t gettid()
        {
            return static_cast<pid_t> (::syscall(SYS_gettid));
        }

        void cacheTid()
        {
            if(t_cachedtid = 0)
            {
                t_cachedtid = gettid();
            }
        }
    }
}

MyNet::base::Thread::Thread(const ThreadFunc& func, const std::string& name)
    : m_func(func), m_name(name), m_started(false), m_joined(false)
{

}

MyNet::base::Thread::~Thread()
{
    if(m_started && !m_joined)
    {
        pthread_detach(m_threadId);
    }
}

void MyNet::base::Thread::start()
{
    assert(!m_started);
    if(!pthread_create(&m_threadId, NULL, runInThread, this))
    {
        // create success
        m_started = true;
    }
}

void MyNet::base::Thread::runInThread(void* thread_obj)
{
    MyNet::base::Thread * thread = static_cast<MyNet::base::Thread*> (thread_obj);
    thread->m_tid = MyNet::ThreadOp::tid();
    thread->m_func();
}

int MyNet::base::Thread::join()
{
    assert(m_started);
    assert(!m_joined);
    return pthread_join(m_threadId, NULL);
}
