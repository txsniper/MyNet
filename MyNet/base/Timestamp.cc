#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#undef __STDC_FORMAT_MACROS
#include <stdio.h>
#include <sys/time.h>
#include "Timestamp.h"
namespace MyNet
{
namespace base
{
    Timestamp::Timestamp(int64_t value)
        : m_microSecsSinceEpoch(value)
    {

    }

    std::string Timestamp::toString() const
    {
        char buff[32] = {0};
        int64_t seconds = m_microSecsSinceEpoch / kMicroSecsPerSecond;
        int64_t microSeconds = m_microSecsSinceEpoch % kMicroSecsPerSecond;
        snprintf(buff, sizeof(buff) - 1, "%" PRId64 ".%06" PRId64 "", seconds, microSeconds);
        return buff;
    }

    std::string Timestamp::toFormatedString() const
    {
        char buff[32] = {0};
        time_t seconds = static_cast<time_t> (m_microSecsSinceEpoch / kMicroSecsPerSecond);
        int microseconds = static_cast<int> (m_microSecsSinceEpoch % kMicroSecsPerSecond);
        struct tm tm_time;
        gmtime_r(&seconds, &tm_time);
        snprintf(buff, sizeof(buff), "%4d%02d%02d %02d:%02d:%02d.%06d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
            microseconds);
        return buff;
    }

    Timestamp Timestamp::now()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int64_t seconds = tv.tv_sec;
        return Timestamp(seconds * kMicroSecsPerSecond + tv.tv_usec);
    }

    Timestamp Timestamp::invalid()
    {
        return Timestamp();
    }
}
}
