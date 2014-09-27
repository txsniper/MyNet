#ifndef _BASE_TIME_STAMP_H_
#define _BASE_TIME_STAMP_H_

#include <stdint.h>
#include <algorithm>
#include <string>
#include <time.h>
#include <boost/operators.hpp>
#include "Copyable.h"

namespace MyNet
{
namespace base
{
    class Timestamp: public MyNet::Copyable,
                     public boost::less_than_comparable<Timestamp>
    {
        public:
            Timestamp()
                : m_microSecsSinceEpoch(0)
            {

            }

            explicit Timestamp(int64_t value);

            void swap(Timestamp& other)
            {
                std::swap(m_microSecsSinceEpoch, other.m_microSecsSinceEpoch);
            }

            std::string toString() const;
            std::string toFormatedString() const;

            bool valid() const
            {
                return (m_microSecsSinceEpoch > 0);
            }

            int64_t microSecsSinceEpoch() const
            {
                return m_microSecsSinceEpoch;
            }

            time_t secsSinceEpoch() const
            {
                return (static_cast<time_t>(m_microSecsSinceEpoch / kMicroSecsPerSecond));
            }

            static Timestamp now();
            static Timestamp invalid();

            static const int kMicroSecsPerSecond = 1000 * 1000;
        private:
            int64_t m_microSecsSinceEpoch;

    };
}
}
#endif
