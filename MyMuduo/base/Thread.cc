#include "Thread.h"
#include "ThreadOpt.h"
#include <pthread.h>
#include <assert.h>

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
    }
}

void MyNet::base::Thread::runInThread(void* thread_obj)
{
    MyNet::base::Thread * thread = static_cast<MyNet::base::Thread*> (thread_obj);
    thread->m_started = true;
    thread->m_func();
}
