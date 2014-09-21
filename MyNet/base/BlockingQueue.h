#ifndef _BASE_BLOCKING_QUEUE_H_
#define _BASE_BLOCKING_QUEUE_H_

#include "Condition.h"
#include "Mutex.h"
#include "NonCopyable.h"
#include <assert.h>
#include <deque>
namespace MyNet
{
namespace base
{
    template<typename T>
    class BlockingQueue: private MyNet::Noncopyable
    {
        public:
            BlockingQueue()
                : m_mutex(), m_cond(m_mutex), m_deque()
            {

            }
            ~BlockingQueue(){}

            T get()
            {
                MyNet::base::MutexLockGuard lock(m_mutex);
                while(m_deque.empty())
                    m_cond.wait();
                assert(!m_deque.empty());
                T value(m_deque.front());
                m_deque.pop_front();
                return T;
            }
            void put(const T& value)
            {
                MyNet::base::MutexLockGuard lock(m_mutex);
                m_deque.push_back(value);
                m_cond.notify();
            }

            size_t size()
            {
                MyNet::base::MutexLockGuard lock(m_mutex);
                return m_deque.size();
            }
        private:
            MyNet::base::MutexLock m_mutex;
            MyNet::base::Condition m_cond;
            std::deque<T> m_deque;
    };
}
}
#endif
