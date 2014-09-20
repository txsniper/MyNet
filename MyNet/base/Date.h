#ifndef _BASE_DATE_H_
#define _BASE_DATE_H_
#include <time.h>
#include <string.h>
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
		Date(const struct& tm);

		// convert to yyyy-mm-dd format 
		std::string toString() const;

		YearMonthDay toYearMonthDay() const;
	private:
		int m_nJulianDayNum;
	};
}
}
#endif