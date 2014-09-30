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

    struct comp
    {
        bool compareGMT;
        comp(bool gmt)
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
        return true;
    }
}
}
