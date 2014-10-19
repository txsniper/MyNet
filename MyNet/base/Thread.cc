#include "Thread.h"
#include "ThreadOp.h"
#include <sys/syscall.h>
#include <pthread.h>
#include <assert.h>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

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
            if(t_cachedtid == 0)
            {
                t_cachedtid = gettid();
            }
        }

        typedef struct ThreadData
        {
            typedef MyNet::base::Thread::ThreadFunc ThreadFunc;
            ThreadFunc m_func;
            std::string m_name;
            boost::weak_ptr<pid_t> m_wktid;

            ThreadData(const ThreadFunc& func,
                        const std::string& name,
                        const boost::shared_ptr<pid_t>& sharedtid)
                : m_func(func), m_name(name), m_wktid(sharedtid)
            {

            }

            void runInThread()
            {
                MyNet::ThreadOp::t_threadname = m_name.c_str();
                pid_t tid = MyNet::ThreadOp::tid();
                boost::shared_ptr<pid_t> ptid = m_wktid.lock();
                if(ptid)
                {
                    *ptid = tid;
                    ptid.reset(); // 上面的lock增加了引用计数，这里reset减小它
                }

                m_func();
            }
        } ThreadData;

        void* startThread(void* thread_data)
        {
            ThreadData* td = static_cast<ThreadData*> (thread_data);
            td->runInThread();
            delete td;
            return NULL;
        }
}
}

MyNet::base::AtomicInt32 MyNet::base::Thread::m_ThreadNum;
MyNet::base::Thread::Thread(const ThreadFunc& func, const std::string& name)
    : m_sharedtid(new pid_t(0)),
      m_name(name),
      m_started(false),
      m_joined(false),
      m_func(func)
{
    m_ThreadNum.incAndGet();
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
    MyNet::ThreadOp::ThreadData* thread_data = new MyNet::ThreadOp::ThreadData(m_func, m_name, m_sharedtid);
    if(!pthread_create(&m_threadId, NULL, MyNet::ThreadOp::startThread, thread_data))
    {
        // create success
        m_started = true;
    }
    else
    {
        delete thread_data;
    }
}


int MyNet::base::Thread::join()
{
    assert(m_started);
    assert(!m_joined);
    m_joined = true;
    return pthread_join(m_threadId, NULL);
}
