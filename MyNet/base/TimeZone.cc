#include "TimeZone.h"
#include "NonCopyable.h"

#include <stdio.h>
#include <string>
#include <endian.h>
#include <stdint.h>
#include <vector>

namespace MyNet
{
namespace base
{
    struct Transition
    {
        time_t gmtime;
        time_t localtime;
        int localtimeIdx;

        Transition(time_t g, time_t l, int index)
            : gmtime(g), localtime(l), localtimeIdx(index)
        {

        }
    };

    struct Comp
    {
        bool compareGMT;
        Comp(bool gmt)
            : compareGMT(gmt)
        {

        }

        bool operator () (const Transition& lhs, const Transition& rhs)
        {
            if(compareGMT)
                return lhs.gmtime < rhs.gmtime;
            else
                return lhs.localtime < rhs.localtime;
        }

        bool equal(const Transition& lhs, const Transition& rhs)
        {
            if(compareGMT)
                return lhs.gmtime == rhs.gmtime;
            else
                return lhs.localtime == rhs.localtime;
        }
        };

        struct LocalTime
        {
            time_t gmtOffset;
            bool isDest;
            int arrbInx;

            LocalTime(time_t offset, bool dest, int index)
                : gmtOffset(offset), isDest(dest), arrbInx(index)
            {

            }
        };

        inline void fillHMS(unsigned int seconds, struct tm* utc)
        {
            utc->tm_sec = seconds % 60;
            unsigned int minutes = seconds / 60;
            utc->tm_min = minutes % 60;
            utc->tm_hour = minutes / 60;
        }

        struct TimeZone::Data
        {
            std::vector<Transition> transitions;
            std::vector<LocalTime>  localtimes;
            std::vector<std::string> names;
            std::string abbreviation;
        };

        const int kSecsPerDay = 24 * 60 * 60;

        class File : MyNet::Noncopyable
        {
            public:
                File(const char* filename)
                    : m_fp(::fopen(filename, "rb"))
                {

                }

                ~File()
                {
                    if(m_fp)
                        ::fclose(m_fp);
                }

                bool valid()
                {
                    return m_fp != NULL;
                }

                std::string readBytes(size_t n)
                {
                    char buff[n];
                    size_t result = ::fread(buff, 1, n, m_fp);
                    if(result != n)
                    {

                    }
                    return std::string(buff, result);
                }

                int32_t readInt32()
                {
                    int32_t x = 0;
                    size_t result = ::fread(&x, 1, sizeof(x), m_fp);
                    if(result != sizeof(x))
                    {

                    }
                    return be32toh(x);
                }

                uint8_t readUint8()
                {
                    uint8_t x = 0;
                    size_t result = ::fread(&x, 1, sizeof(x), m_fp);
                    if(result != sizeof(x))
                    {

                    }
                    return x;
                }

            private:
                FILE* m_fp;
        };

        bool readTimeZoneFile(const char* zonefile, struct TimeZone::Data* data)
        {
            File f(zonefile);
            if(f.valid())
            {
                std::string head = f.readBytes(4);
                if(head != "TZif")
                    return false;
                std::string version = f.readBytes(1);
                f.readBytes(15);

                int32_t isgmtcnt = f.readInt32();
                int32_t isstdcnt = f.readInt32();
                int32_t leapcnt = f.readInt32();
                int32_t timecnt = f.readInt32();
                int32_t typecnt = f.readInt32();
                int32_t charcnt = f.readInt32();

                std::vector<int32_t> trans;
                std::vector<int> localtimes;
                trans.reserve(timecnt);
                for(int i = 0; i < timecnt; ++i)
                {
                    trans.push_back(f.readInt32());
                }

                for(int i = 0; i < timecnt; ++i)
                {
                    uint8_t local = f.readUint8();
                    localtimes.push_back(local);
                }

                for(int i = 0; i < typecnt; ++i)
                {
                    int32_t gmtoff = f.readInt32();
                    uint8_t isdst = f.readUint8();
                    uint8_t abbrind = f.readUint8();
                    data->localtimes.push_back(LocalTime(gmtoff, isdst, abbrind));
                }

                for(int i = 0; i < timecnt; ++i)
                {
                    int localIdx = localtimes[i];
                    time_t localtime = trans[i] + data->localtimes[localIdx].gmtOffset;
                    data->transitions.push_back(Transition(trans[i], localtime, localIdx));
                }

                data->abbreviation = f.readBytes(charcnt);
                for(int i = 0; i < leapcnt; ++i)
                {

                }

                (void) isstdcnt;
                (void) isgmtcnt;
            }
            return true;

        }

        const LocalTime* findLocalTime(const TimeZone::Data& data, Transition sentry, Comp comp)
        {
            const LocalTime* local = NULL;
            if(data.transitions.empty() || comp(sentry, data.transitions.front()))
            {
                local = &data.localtimes.front();
            }
            else
            {
                std::vector<Transition>::const_iterator transI = lower_bound(data.transitions.begin(),
                                                                            data.transitions.end(),
                                                                            sentry,
                                                                            comp);
                if(transI != data.transitions.end())
                {
                    if(!comp.equal(sentry, *transI))
                    {
                        assert(transI != data.transitions.begin());
                        --transI;
                    }
                    local = &data.localtimes[transI->localtimeIdx];
                }
                else
                {
                    local = &data.localtimes[data.transitions.back().localtimeIdx];
                }
            }
            return local;
        }

        TimeZone::TimeZone(const char* timezone_file)
            : m_spDate(new TimeZone::Data)
        {
            if(!readTimeZoneFile(timezone_file, m_spDate.get()))
            {
                m_spDate.reset();
            }
        }

        TimeZone::TimeZone(int eastofUtc, const char* tzname)
            : m_spDate(new TimeZone::Data)
        {
            m_spDate->localtimes.push_back(LocalTime(eastofUtc, false, 0));
            m_spDate->abbreviation = tzname;
        }

        struct tm TimeZone::toLocalTime(time_t secsSinceEpoch) const
        {
            struct tm tm_temp;
            return tm_temp;
        }
}
}
