#include <iostream>
#include "datetime.h"
#include <string>
#include <vector>

int main()
{
	// Getting started //

	// Getting the current time.
	auto now = EZ::Datetime::now();
	std::cout << "Now is " << now << std::endl;
	// >> Now is 2021/04/25 22:02:00 JST

	// Set datetimes.
	auto date0 = EZ::Datetime("2021/1/1 0:00:00" /*, false*/);
	auto date1 = EZ::Datetime("2021/1/1 0:00:00", "%Y/%m/%d %H:%M:%S" /*, false*/);
	auto date2 = EZ::Datetime(2022, 1, 1, 0, 0, 0 /*, false*/);

	// Show datetimes.
	std::cout << "date0 is " << date0 << std::endl;
	std::cout << "date1 is " << date1 << std::endl;
	std::cout << "date2 is " << date2.str("西暦%Y年%m月%d日 %H時%M分%S秒 TZ=%Z") << std::endl;
	// >> date0 is 2021/01/01 00:00:00 JST
	// >> date1 is 2021/01/01 00:00:00 JST
	// >> date2 is 西暦2022年01月01日 00時00分00秒 TZ=JST

	// Culculation of difference of two datetimes.
	auto timedelta = date2 - date1;
	std::cout << date2 << " - " << date1 << "\n\t = " << timedelta << std::endl;
	// >> 2022/01/01 00:00:00 JST - 2021/01/01 00:00:00 JST
	// >>	 = TimeDelta(days=365, hours=0, minutes=0, seconds=0)

	// Set TimeDelta objects.
	auto delta0 = EZ::TimeDelta(-365 * 24 * 3600);
	auto delta1 = EZ::TimeDelta(365, 0, 0, 0);

	std::cout << "delta0 = " << delta0 << std::endl;
	std::cout << "delta1 = " << delta1 << std::endl;
	// >> delta0 = TimeDelta(days=-365, hours=0, minutes=0, seconds=0)
	// >> delta1 = TimeDelta(days=365, hours=0, minutes=0, seconds=0)

	// Arithmetic operations.
	// Add, Sub
	std::cout << "delta0 + delta1 = " << delta1 + delta1 << std::endl;
	std::cout << "delta0 - delta1 = " << delta1 - delta1 << std::endl;
	// Multiply, divide
	std::cout << "delta1 * 2.0 = " << delta1 * 2 << std::endl;
	std::cout << "delta1 / 2.0 = " << delta1 / 2 << std::endl;

	// >> delta0 + delta1 = TimeDelta(days=730, hours=0, minutes=0, seconds=0)
	// >> delta0 - delta1 = TimeDelta(days=0, hours=0, minutes=0, seconds=0)
	// >> delta1 * 2.0 = TimeDelta(days=730, hours=0, minutes=0, seconds=0)
	// >> delta1 / 2.0 = TimeDelta(days=182, hours=12, minutes=0, seconds=0)

	// Example of exception.
	try
	{
		// Year is Negative.
		EZ::Datetime err = EZ::Datetime(-2000, -1, -1, 0, 0, 0);
	}
	catch (EZ::DatetimeException e)
	{
		std::cout << "Error message: " << e.what() << std::endl;
	}
	// >> Error message: Input time is out of range. minimum datetime is 1970/01/01 09:00:00 JST

	return 0;
}
