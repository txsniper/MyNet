#include "LogFile.h"
namespace MyNet
{

namespace base
{
    LogFile::LogFile(bool threadsafe,
                    size_t rollsize,
                    const std::string& basename,
                    int flushInterval,
                    int checkEveryN)
        : m_mutexLock(threadsafe ?new MutexLock() : NULL),
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
            MyNet::base::MutexLockGuard(*m_mutexLock);
            append_unlocked(line, n);
        }
        else
        {
            append_unlocked(line, n);
        }
    }

    void LogFile::append_unlocked(const char* line, size_t n)
    {
        m_scpFile->writeLine(line, n);
    }

    std::string LogFile::getLogFileName(const std::string& basename, time_t* now)
    {
        std::string filename;
        filename.reserve(basename.size() + 64);
        filename = basename;
        char tmpbuffer[32] = {0};
        struct tm tm;
        *now = time(NULL);
        gmtime_r(*now, &tm);
        strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S.", &tm);
        filename += timebuf;
        filename += ProcessInfo::hostname();
        char pidbuf[32];
        snprintf(pidbuf, sizeof pidbuf, ".%d", ProcessInfo::pid());
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
