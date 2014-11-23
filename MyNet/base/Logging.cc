#include "Logging.h"
#include "ThreadOp.h"
#include "TimeZone.h"
#include "Timestamp.h"
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

namespace MyNet
{

    namespace base
    {

        __thread char t_errnobuf[512];
        __thread char t_time[32];
        __thread time_t t_lastSecond;

        //获取当前线程的出错原因
        const char* strerror_tl(int savedErrno)
        {
            return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
        }

        // 根据环境变量设定当前的日志级别
        Logger::LogLevel initLogLevel()
        {
            if(::getenv("MYNET_LOG_TRACE"))
                return Logger::TRACE;
            else if (::getenv("MYNET_LOG_DEBUG"))
                return Logger::DEBUG;
            else
                return Logger::INFO;
        }
        Logger::LogLevel g_logLevel = initLogLevel();

        const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
        {
            "TRACE ",
            "DEBUG ",
            "INFO  ",
            "WARN  ",
            "ERROR ",
            "FATAL ",
        };

        class T
        {
            public:
                T(const char* str, size_t len)
                    : str_(str), len_(len)
                {
                    assert(strlen(str) == len_);
                }
                const char* str_;
                const size_t len_;
        };

        inline LogStream& operator << (LogStream& s, T v)
        {
            s.append(v.str_, v.len_);
            return s;
        }

        inline LogStream& operator << (LogStream& s, const Logger::SourceFile& v)
        {
            s.append(v.m_data, v.m_size);
            return s;
        }

        void defaultOutput(const char* msg, size_t len)
        {
            size_t n = fwrite(msg, 1, len, stdout);
            (void)n;
        }

        void defaultFlush()
        {
            fflush(stdout);
        }


        Logger::OutputFunc g_output = defaultOutput;
        Logger::FlushFunc g_flush = defaultFlush;
        TimeZone g_logTimeZone;

        Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line)
            : m_time(Timestamp::now()),
            m_stream(),
            m_level(level),
            m_line(line),
            m_basename(file)
        {
            formatTime();
            // ThreadOp::tid()函数调用cachetid将线程名字和长度保存在
            // t_tidString和t_tidStringLength中
            ThreadOp::tid();
            m_stream<< T(ThreadOp::tidString(), ThreadOp::tidStringLength());
            m_stream<< T(LogLevelName[level], 6);
            if (savedErrno != 0)
            {
                m_stream<< strerror_tl(savedErrno) << "(errno=" << savedErrno << ")";
            }
        }

        void Logger::Impl::formatTime()
        {
            int64_t microSecondsSinceEpoch = m_time.microSecsSinceEpoch();
            time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / Timestamp::kMicroSecsPerSecond);
            int microseconds = static_cast<int>(microSecondsSinceEpoch % Timestamp::kMicroSecsPerSecond);
            if (seconds != t_lastSecond)
            {
                t_lastSecond = seconds;
                struct tm tm_time;
                if (g_logTimeZone.valid())
                {
                    tm_time = g_logTimeZone.toLocalTime(seconds);
                }
                else
                {
                    ::gmtime_r(&seconds, &tm_time); // FIXME TimeZone::fromUtcTime
                }

                int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                        tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                        tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
                assert(len == 17); (void)len;
            }

            if (g_logTimeZone.valid())
            {
                Fmt us(".%06d ", microseconds);
                assert(us.length() == 8);
                m_stream << T(t_time, 17) << T(us.data(), 8);
            }
            else
            {
                Fmt us(".%06dZ ", microseconds);
                assert(us.length() == 9);
                m_stream << T(t_time, 17) << T(us.data(), 9);
            }
        }

        void Logger::Impl::finish()
        {
            m_stream << "-" << m_basename << ":" << m_line << "\n";
        }

        Logger::Logger(Logger::SourceFile file, int line)
            : m_impl(INFO, 0, file, line)
        {

        }

        Logger::Logger(Logger::SourceFile file, int line, LogLevel level)
            : m_impl(level, 0, file, line)
        {

        }

        Logger::Logger(SourceFile file, int line, bool toAbort)
            : m_impl(toAbort?FATAL:ERROR, errno, file, line)
            {
            }

        Logger::~Logger()
        {
            m_impl.finish();
            const LogStream::Buffer& buf(stream().buffer());
            g_output(buf.data(), buf.length());
            if (m_impl.m_level == FATAL)
            {
                g_flush();
                abort();
            }
        }

        void Logger::setLogLevel(Logger::LogLevel level)
        {
            g_logLevel = level;
        }

        void Logger::setOutput(OutputFunc out)
        {
            g_output = out;
        }

        void Logger::setFlush(FlushFunc flush)
        {
            g_flush = flush;
        }

        void Logger::setTimeZone(const TimeZone& tz)
        {
            g_logTimeZone = tz;
        }

    }
}
