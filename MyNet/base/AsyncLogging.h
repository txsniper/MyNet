#ifndef _BASE_ASYNCLOGGING_H_
#define _BASE_ASYNCLOGGING_H_
#include "NonCopyable.h"
#include "Thread.h"
#include "BoundedBlockingQueue.h"
#include "Mutex.h"
#include "CountDownLatch.h"
#include "Condition.h"
#include "LogStream.h"

#include <string.h>
#include <boost/ptr_container/ptr_vector.hpp>

namespace MyNet
{
namespace base
{

    class AsyncLogging : MyNet::Noncopyable
    {
        public:
            AsyncLogging(const std::string& basename, size_t rollSize, int flushInterval = 3);
            ~AsyncLogging()
            {
                if(m_running)
                {
                    stop();
                }
            }

            void append(const char* logline, size_t length);
        private:

            // 必须将复制构造函数和复制操作符声明为private，否则boost编译报错
            AsyncLogging(const AsyncLogging& );
            void operator = (const AsyncLogging&);

            void ThreadFunc();

            void start();
            void stop();

            typedef FixedBuffer<kLargeBuffer> Buffer;
            typedef boost::ptr_vector<Buffer> BuffVec;
            typedef BuffVec::auto_type BuffPtr;

            size_t m_rollSize;
            int m_flushInterval;

            std::string m_basename;
            bool m_running;
            Thread m_loggingThread;

            CountDownLatch m_latch;
            MutexLock m_mutex;
            Condition m_cond;

            BuffPtr m_curbuff;
            BuffPtr m_nextbuff;
            BuffVec m_buffs;

    };
}
}
#endif
