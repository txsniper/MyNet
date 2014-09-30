#ifndef _BASE_TIME_ZONE_H_
#define _BASE_TIME_ZONE_H_

#include <boost/shared_ptr.hpp>
#include "Copyable.h"
#include <time.h>
namespace MyNet
{
namespace base
{

    class TimeZone : public MyNet::Copyable
    {
        public:
            explicit TimeZone(const char* zone_file);
            TimeZone(int eastofUt, const char* tzname);
            TimeZone() {}

            bool valid()
            {
                if(m_spDate)
                    return true;
                else
                    return false;
            }

            struct tm toLocalTime(time_t secsSinceEpoch) const;
            time_t fromLocalTime(const struct tm&) const;

            static struct tm toUTCTime(time_t secsSinceEpoch, bool yday = false);
            static time_t fromUTCTime(const struct tm&);
            static time_t fromUTCTime(int year, int month, int day,
                                      int hour, int minute, int seconds);
            struct Data;
        private:
            boost::shared_ptr<Data> m_spDate;

    };
}
}
#endif
