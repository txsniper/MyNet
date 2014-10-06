#ifndef _BASE_THREAD_POOL_H_
#define _BASE_THREAD_POOL_H_

#include "NonCopyable.h"
#include "Thread.h"
#include "Mutex.h"
#include "Condition.h"

#include <boost/function.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <deque>
#include <string>

namespace MyNet
{
namespace base
{

    class ThreadPool : private MyNet::Noncopyable
    {
        public:
            typedef boost::function<void()> Task;
            explicit ThreadPool(const std::string& name = std::string("ThreadPool"));
            ~ThreadPool();
            void setMaxQueueSize(size_t new_size)
            {
                m_maxQueueSize = new_size;
            }

            void setThreadInitFunc(const Task& cb)
            {
                m_ThreadInitFunc = cb;
            }

            void start(int numfoThreads);
            void stop();
            void run(const Task& task);
        private:
            bool isFull() const;
            Task getTask();
            void runInThread();

            std::string m_name;
            MutexLock m_Mutex;
            Condition m_notFull;
            Condition m_notEmpty;
            bool m_brunning;
            Task m_ThreadInitFunc;
            boost::ptr_vector<MyNet::base::Thread> m_threads;
            std::deque<Task> m_tasks;
            size_t m_maxQueueSize;
    };
}
}
#endif
