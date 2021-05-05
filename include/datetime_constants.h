#ifndef _MY_DATETIME_CONSTANTS_
#define _MY_DATETIME_CONSTANTS_

#include <string>
#include <vector>
#include <limits>

namespace DatetimeConstants
{
	const std::string DEFAULT_INPUT_FORMAT = "%Y/%m/%d %H:%M:%S";
	const std::string DEFAULT_OUTPUT_FORMAT = "%Y/%m/%d %H:%M:%S %Z";
	const int TM_BASE_YEAR = 1900;
	const int MONTH_OFFSET = 1;

	// https://docs.microsoft.com/ja-jp/cpp/c-runtime-library/reference/gmtime-gmtime32-gmtime64?view=msvc-160
	const int MINIMUM_SEC = 0;
	const long long MAXIMUM_SEC = 32503766400; // at 3000/1/2 00:00:00 UTC

	double DOUBLE_EPSILON = std::numeric_limits<double>::epsilon();
}
#endif