#include <boost/scoped_ptr.hpp>
#include <time.h>
#include <string.h>
#include "FileUtil.h"
#include "NonCopyable.h"
#include "Mutex.h"

namespace MyNet
{

namespace base
{
    class LogFile : private MyNet::Noncopyable
    {
        public:
            LogFile(bool threadsafe, size_t rollsize,
                    const std::string& basename, int flushInterval
                    );
        private:
            boost::scoped_ptr<base::MutexLock> m_mutexLock;
            boost::scoped_ptr<base::WriteFile> m_scpFile;

            time_t m_startPeriod;   // 日志文件创建时间
            time_t m_lastFlush;     // 上一次写入磁盘的时间
            time_t m_lastRoll;      // 上一次回滚时间
            bool m_bThreadSafe;
            const size_t m_rollSize;        // 日志文件回滚的大小
            const std::string m_basename;   // 日志文件基础名字
            const int m_flushInterval;      // 写到硬盘的时间间隔
            const int m_checkEveryN;        // 经历m_checkEveryN次append后检查是否roll
            const static int m_rollPeriod = 24 * 60 * 60; // 日志文件回滚周期
    };
}
}
