#ifndef _BASE_DATE_H_
#define _BASE_DATE_H_
#include <time.h>
#include <string>
#include <stdint.h>
namespace MyNet
{
namespace base
{
	class Date
	{
	public:
		typedef struct YearMonthDay_
		{
			int year;
			int month;
			int day;
		} YearMonthDay;
		static const int NumDaysPerWeek = 7;
		static const int JulianDayOf1970_01_01;
		Date()
		 : m_nJulianDayNum(0)
		{

		}

		Date(int nJulianDayNum)
		 : m_nJulianDayNum(nJulianDayNum)
		{

		}

		Date(int year, int month, int day);
		Date(const struct tm&);

		// convert to yyyy-mm-dd format 
		std::string toString() const;

		YearMonthDay toYearMonthDay() const;

        int julianDayNumber() const
        {
            return m_nJulianDayNum;
        }

        int year() const
        {
            return toYearMonthDay().year;
        }

        int month() const
        {
            return toYearMonthDay().month;
        }

        int day() const
        {
            return toYearMonthDay().day;
        }

        bool valid() const
        {
            return (m_nJulianDayNum > 0);
        }

        int weekDay() const
        {
            return (julianDayNumber() + 1) % NumDaysPerWeek;
        }
	private:
		int m_nJulianDayNum;
	};

    inline bool operator < (Date a, Date b)
    {
        return a.julianDayNumber() < b.julianDayNumber();
    }

    inline bool operator == (Date a, Date b)
    {
        return a.julianDayNumber() == b.julianDayNumber();
    }
}
}
#endif
