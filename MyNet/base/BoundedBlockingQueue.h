#ifndef _BASE_BOUNDED_BLOCKING_QUEUE_H_
#define _BASE_BOUNDED_BLOCKING_QUEUE_H_

#include <boost/circular_buffer.hpp>
#include "NonCopyable.h"
#include "Mutex.h"
#include "Condition.h"
#include <assert.h>

namespace MyNet
{
namespace base
{
    template <typename T>
    class BoundedBlockingQueue : private MyNet::Noncopyable
    {
        public:
            BoundedBlockingQueue(int maxSize)
                : m_queue(maxSize), m_mutex(), m_full(m_mutex), m_empty(m_mutex)
            {

            }

            ~BoundedBlockingQueue() {}

            void put(const T& value)
            {
                MyNet::base::MutexLockGuard guard(m_mutex);
                while(m_queue.full())
                {
                    m_full.wait();
                }
                assert(!m_queue.full());
                m_queue.push_back(value);
                m_empty.notify();
            }

            T get()
            {
                MyNet::base::MutexLockGuard guard(m_mutex);
                while(m_queue.empty())
                {
                    m_empty.wait();
                }
                assert(!m_queue.empty());
                T value(m_queue.front());
                m_queue.pop_front();
                m_full.notify();
                return value;
            }

            bool empty() const
            {
                MyNet::base::MutexLockGuard guard(m_mutex);
                return m_queue.empty();
            }

            bool capacity() const
            {
                MyNet::base::MutexLockGuard guard(m_mutex);
                return m_queue.capacity();
            }

            size_t size() const
            {
                MyNet::base::MutexLockGuard guard(m_mutex);
                return m_queue.size();
            }
        private:
            boost::circular_buffer<T> m_queue;
            mutable MyNet::base::MutexLock m_mutex;
            MyNet::base::Condition m_full;
            MyNet::base::Condition m_empty;
    };
}
}
#endif
