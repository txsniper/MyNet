#include "ThreadPool.h"

#include <boost/bind.hpp>
#include <assert.h>
#include <stdio.h>

namespace MyNet
{
namespace base
{
    ThreadPool::ThreadPool(const std::string& name)
        : m_name(name),
        m_Mutex(),
        m_notFull(m_Mutex),
        m_notEmpty(m_Mutex),
        m_brunning(false)
    {

    }

    ThreadPool::~ThreadPool()
    {
        if(m_brunning)
        {
            stop();
        }
    }

    void ThreadPool::start(int numfoThreads)
    {
        assert(m_threads.empty());
        m_brunning = true;
        m_threads.reserve(numfoThreads);
        for(int i = 0; i < numfoThreads; ++i)
        {
            char id[32] = {0};
            snprintf(id, sizeof(id), "%d", i+1);
            m_threads.push_back(new MyNet::base::Thread(boost::bind(&ThreadPool::runInThread, this), m_name + id));
            m_threads[i].start();
        }
        if(numfoThreads == 0 && m_ThreadInitFunc)
        {
            m_ThreadInitFunc();
        }
    }

    void ThreadPool::stop()
    {
        {
            MyNet::base::MutexLockGuard lock(m_Mutex);
            m_brunning = false;
            // 让阻塞在m_tasks上的任务线程解除阻塞
            m_notEmpty.notifyAll();
        }
        for_each(m_threads.begin(),
                 m_threads.end(),
                 boost::bind(&MyNet::base::Thread::join, _1));

    }
    void ThreadPool::runInThread()
    {
        if(m_ThreadInitFunc)
        {
            m_ThreadInitFunc();
        }
        while(m_brunning)
        {
            Task task = getTask();
            if(task)
            {
                task();
            }
        }
    }
    void ThreadPool::run(const Task& task)
    {
        if(m_threads.empty())
        {
            Task();
        }
        else
        {
            MyNet::base::MutexLockGuard lock(m_Mutex);
            while(isFull())
            {
                m_notFull.wait();
            }
            m_tasks.push_back(task);
            m_notEmpty.notify();
        }
    }

    ThreadPool::Task ThreadPool::getTask()
    {
        MyNet::base::MutexLockGuard lock(m_Mutex);
        while(m_tasks.empty() && m_brunning)
        {
            m_notEmpty.wait();
        }
        Task task;
        if(!m_tasks.empty())
        {
            task = m_tasks.front();
            m_tasks.pop_front();
            m_notFull.notify();
        }
        return task;
    }


    // 外部加锁
    bool ThreadPool::isFull() const
    {
        if(m_tasks.size() >= m_maxQueueSize)
            return true;
        return false;
    }
}
}
