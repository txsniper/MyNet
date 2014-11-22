#ifndef _BASE_THREAD_H_
#define _BASE_THREAD_H_

#include <string.h>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "NonCopyable.h"
#include "Atomic.h"
namespace MyNet
{
namespace base
{
    class Thread : private MyNet::Noncopyable
    {
        public:
            typedef boost::function<void()> ThreadFunc;
            Thread(const ThreadFunc& func, const std::string& name);
            ~Thread();
            void start();
            int join();
            pid_t tid() const
            {
                return *m_sharedtid;
            }

            bool started() const
            {
                return m_started;
            }

            static int threadNum()
            {
                return m_ThreadNum.get();
            }

            void setDefaultName();
        private:
            pthread_t   m_threadId;                 // pthread id, used in pthread functions
            boost::shared_ptr<pid_t> m_sharedtid;   // thread id, used by linux system ,it's different from pthread id
            std::string m_name;
            bool        m_started;         // Wether the thread has started or not
            bool        m_joined;
            ThreadFunc  m_func;

            static MyNet::base::AtomicInt32 m_ThreadNum;
    };
}
}
#endif
