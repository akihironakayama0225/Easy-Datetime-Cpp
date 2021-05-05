#ifndef _MY_UNIX_TIME_
#define _MY_UNIX_TIME_

#include <string>
#include <regex>
#include <vector>
#include <time.h>
#include <sstream>
#include <iomanip>

#include "datetime_exceptions.h"
#include "datetime_constants.h"

namespace EZ
{
	namespace MyTM
	{
		bool operator==(const struct tm &tm1, const struct tm &tm2)
		{
			// Year,Month,Day, hour,minutes,seconds の等価演算

			if (tm1.tm_year != tm2.tm_year)
			{
				return false;
			}
			if (tm1.tm_mon != tm2.tm_mon)
			{
				return false;
			}
			if (tm1.tm_mday != tm2.tm_mday)
			{
				return false;
			}
			if (tm1.tm_hour != tm2.tm_hour)
			{
				return false;
			}
			if (tm1.tm_min != tm2.tm_min)
			{
				return false;
			}
			if (tm1.tm_sec != tm2.tm_sec)
			{
				return false;
			}
			return true;
		}

		bool operator!=(const struct tm &tm1, const struct tm &tm2)
		{
			return !(operator==(tm1, tm2));
		}

		// 仕様: サマータイムの設定は struct tmの仕様に準拠する
		// mktime()のサマータイム対策用関数
		// https://stackoverflow.com/questions/12122084/confusing-behaviour-of-mktime-function-increasing-tm-hour-count-by-one
		// https://stackoverflow.com/questions/8558919/mktime-and-tm-isdst
		time_t my_mktime(const struct tm &time, const bool &isUTC)
		{
			auto time2 = time;
			//time2.tm_isdst = 0;
			//tzset();
			// https://stackoverflow.com/questions/16647819/timegm-cross-platform
			// https://stackoverflow.com/questions/8666378/detect-windows-or-linux-in-c-c/33088568
			// https://web.archive.org/web/20191012035921/http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system

			time_t unixTime;
			if (isUTC)
			{
#if defined(_WIN32) || defined(_WIN64)
				unixTime = _mkgmtime(&(time2));
#else
				unixTime = timegm(&(time2));
#endif
			}
			else
			{
#if defined(_WIN32) || defined(_WIN64)
				unixTime = mktime(&(time2));
#else
				unixTime = timelocal(&(time2));
#endif
			}
			if (time != time2)
			{
				//std::cout << time.tm_year << "<>" <<time2.tm_year << std::endl;
				//std::cout << time.tm_mon << "<>" <<time2.tm_mon << std::endl;
				//std::cout << time.tm_mday << "<>" <<time2.tm_mday << std::endl;
				//std::cout << time.tm_hour << "<>" <<time2.tm_hour << std::endl;
				//std::cout << time.tm_min << "<>" <<time2.tm_min << std::endl;
				//std::cout << time.tm_sec << "<>" <<time2.tm_sec << std::endl;
				throw DatetimeException("Invalid input to mktime() !. Check input args.");
			}
			return unixTime;
		}

		struct tm my_mkStructTm(const time_t &unixTime, const bool &isUTC)
		{
			// http://www.orchid.co.jp/computer/cschool/CREF/gmtime.html
			struct tm retTm;
			if (isUTC)
			{
				retTm = *gmtime(&unixTime);
			}
			else
			{
				retTm = *localtime(&unixTime);
			}
			return retTm;
		}
	}
}
#endif
