#include "Date.h"
#include <assert.h>
#include <stdio.h>
namespace MyNet
{
namespace DateOp
{
	int getJulianDayNum(int year, int month, int day)
	{
		// make sure at least 32bit
		assert(sizeof(int) >= sizeof(int32_t));
		int a = (14 - month) / 12;
  		int y = year + 4800 - a;
  		int m = month + 12 * a - 3;
  		return day + (153*m + 2) / 5 + y*365 + y/4 - y/100 + y/400 - 32045;
	}

	MyNet::base::Date::YearMonthDay getYearMonthDay(int julianDayNumber)
	{
  		int a = julianDayNumber + 32044;
  		int b = (4 * a + 3) / 146097;
  		int c = a - ((b * 146097) / 4);
  		int d = (4 * c + 3) / 1461;
  		int e = c - ((1461 * d) / 4);
  		int m = (5 * e + 2) / 153;
  		Date::YearMonthDay ymd;
  		ymd.day = e - ((153 * m + 2) / 5) + 1;
  		ymd.month = m + 3 - 12 * (m / 10);
  		ymd.year = b * 100 + d - 4800 + (m / 10);
  		return ymd;
	}
}
	MyNet::base::Date::JulianDayOf1970_01_01 = getYearMonthDay(1970,1,1);

namespace base
{
	Date::Date(int year, int month, int day)
	 : m_nJulianDayNum(getJulianDayNum(year, month, day))
	{

	}

	Date::Date(const struct& tm t)
	 : m_nJulianDayNum(getJulianDayNum
	 	(
	 		t.t.tm_year+1900,
     		t.tm_mon+1,
        	t.tm_mday
        ))
	{

	}

	std::string Date::toString() const
	{
		char buf[32] = {0};
		YearMonthDay ymd(toYearMonthDay());
		snprintf(buf, sizeof(buf), "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day)
		return buf;
	}

	Date::YearMonthDay toYearMonthDay() const
	{
		return getYearMonthDay(m_nJulianDayNum);
	}
}
}