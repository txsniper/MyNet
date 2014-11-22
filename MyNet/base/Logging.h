#include "LogStream.h"
#include "Timestamp.h"

namespace MyNet
{

namespace base
{
class TimeZone;

class Logger
{
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_LOG_LEVELS,
        };

        // 代表源文件
        class SourceFile
        {
            public:
                template<int N>
                inline SourceFile(const char (&arr)[N])
                    : m_data(arr),
                      m_size(N-1)
                {
                    const char* slash = strrchr(m_data, '/');
                    if (slash)
                    {
                        m_data = slash + 1;
                        m_size -= static_cast<int>(m_data - arr);
                    }
                }

                explicit SourceFile(const char* filename)
                    : m_data(filename)
                {
                    const char* slash = strchr(filename, '/');
                    if (slash)
                    {
                        m_data = slash + 1;
                    }
                    m_size = static_cast<int> (strlen(m_data));
                }

                const char* m_data;
                int m_size;
        };

        Logger(SourceFile file, int line);
        Logger(SourceFile file, int line, LogLevel level);
        Logger(SourceFile file, int line, LogLevel level, const char* func);
        Logger(SourceFile file, int line, bool toAbort);
        ~Logger();

        LogStream& stream() {return m_impl.m_stream; }

        static LogLevel logLevel();
        static void setLogLevel(LogLevel level);

        typedef void (*OutputFunc)(const char* msg, size_t len);
        typedef void (*FlushFunc)();
        static void setOutput(OutputFunc);
        static void setFlush(FlushFunc);
        static void setTimeZone(const TimeZone& tz);

    private:
        class Impl
        {
            public:
                typedef Logger::LogLevel LogLevel;
                Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
                void formatTime();
                void finish();

                Timestamp m_time;
                LogStream m_stream;
                LogLevel m_level;
                int m_line;
                SourceFile m_basename;
        };

        Impl m_impl;
};

extern Logger::LogLevel g_logLevel;
inline Logger::LogLevel Logger::logLevel()
{
    return g_logLevel;
}

#define LOG_TRACE if (Logger::logLevel() <= Logger::TRACE) \
    Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()
}
}
