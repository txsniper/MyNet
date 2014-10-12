#include "LogFile.h"
namespace MyNet
{

namespace base
{
    LogFile::LogFile(bool threadsafe, size_t rollsize,
                    const std::string& basename,
                    int flushInterval)
        : m_mutexLock(new MutexLock()),
          m_scpFile(new base::WriteFile(basename)),

}
}
