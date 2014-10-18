#include "LogFile.h"
#include "ProcessInfo.h"
namespace MyNet
{

namespace base
{
    LogFile::LogFile(bool threadsafe,
                    size_t rollsize,
                    const std::string& basename,
                    int flushInterval,
                    int checkEveryN)
        : m_mutexLock(threadsafe ?new base::MutexLock() : NULL),
          m_scpFile(new base::WriteFile(basename)),
          m_bThreadSafe(threadsafe),
          m_rollSize(rollsize),
          m_basename(basename),
          m_flushInterval(flushInterval),
          m_checkEveryN(checkEveryN)
    {
        rollFile();
    }

    bool LogFile::append(const char* line, size_t n)
    {
        if( m_mutexLock )
        {
            MyNet::base::MutexLockGuard lock(*m_mutexLock);
            append_unlocked(line, n);
            return true;
        }
        else
        {
            append_unlocked(line, n);
            return true;
        }
    }

    void LogFile::flush()
    {
        if( m_mutexLock )
        {
            MyNet::base::MutexLockGuard lock(*m_mutexLock);
            m_scpFile->flush();
        }
        else
        {
            m_scpFile->flush();
        }
    }

    void LogFile::append_unlocked(const char* line, size_t n)
    {
        m_scpFile->writeLine(line, n);
        if(m_scpFile->size() >= m_rollSize)
        {
            rollFile();
        }
        else
        {
            ++m_appendCount;
            if( m_appendCount >= m_checkEveryN)
            {
                m_appendCount = 0;
                time_t now = ::time(NULL);
                time_t thisPeriod = now / m_rollPeriod * m_rollPeriod;
                if(thisPeriod != m_startPeriod)
                {
                    rollFile();
                }
                else if (now - m_lastFlush >= m_flushInterval)
                {
                    m_lastFlush = now;
                    flush();
                }
            }
        }
    }

    std::string LogFile::getLogFileName(const std::string& basename, time_t* now)
    {
        std::string filename;
        filename.reserve(basename.size() + 64);
        filename = basename;
        char tmpbuffer[32] = {0};
        struct tm tm;
        *now = ::time(NULL);
        gmtime_r(now, &tm);
        strftime(tmpbuffer, sizeof tmpbuffer, ".%Y%m%d-%H%M%S.", &tm);
        filename += tmpbuffer;
        filename += MyNet::ProcInfo::hostname();
        char pidbuf[32];
        snprintf(pidbuf, sizeof pidbuf, ".%d", MyNet::ProcInfo::pid());
        filename += pidbuf;
        filename += ".log";
        return filename;

    }
    bool LogFile::rollFile()
    {
        time_t now = 0;
        std::string filename = getLogFileName(m_basename, &now);
        time_t start = now / m_rollPeriod * m_rollPeriod;
        if (now > m_lastRoll)
        {
            m_lastRoll = now;
            m_lastFlush = now;
            m_startPeriod = start;
            m_scpFile.reset(new MyNet::base::WriteFile(filename));
            return true;
        }
        return false;
    }

}
}
