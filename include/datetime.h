/*
MIT License

Copyright (c) 2021 Akihiro Nakayama

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*! \mainpage Welcome to Eazy Datetime documentation!
 *
 * \section Class-list
 * Click the above [Classes] tab to see the detailed information.
*/

#ifndef _MY_DATETIME_
#define _MY_DATETIME_

#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <regex>
#include <vector>
#include <limits>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "time_delta.h"
#include "unix_time.h"
#include "datetime_parser.h"
#include "datetime_constants.h"
#include "datetime_exceptions.h"

namespace EZ
{
	/**
	* @brief Datetime object
	* @details A datetime object handles date and time information.
	*/
	class Datetime
	{
		time_t m_unixTime = 0;
		bool m_isUTC = false;
		MyParser m_parser;

	public:
		/**
		* @param[in] timestamp ex: 2021/3/8 0:00:15 \n
		* @param[in] format	 ex: %Y/%m/%d %H:%M:%S \n
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		* @details Timestamp and format must have same delimiters.\n ex: auto time = EZ::Datetime("2021/3/8 0:00:15", "%Y/%m/%d %H:%M:%S", true); 
		*/
		Datetime(const std::string &timestamp, const std::string &format, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, format);
		}

		Datetime(const std::string &timestamp, const char *format, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, format);
		}

		Datetime(const char *timestamp, const std::string &format, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, format);
		}

		Datetime(const char *timestamp, const char *format, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, format);
		}

		/**
		* @param[in] timestamp ex: 2021/3/8 0:00:15 \n
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		* @details Format of timestamp is must be "%Y/%m/%d %H:%M:%S". \n ex: auto time = EZ::Datetime("2021/3/8 0:00:15", true); 
		*/
		Datetime(const std::string &timestamp, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, DatetimeConstants::DEFAULT_INPUT_FORMAT);
		}

		Datetime(const char *timestamp, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			setDateTime(timestamp, DatetimeConstants::DEFAULT_INPUT_FORMAT);
		}

		/**
		* @param[in] datetime struct tm
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		*/
		Datetime(struct tm datetime, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			validateInput(datetime);
			m_unixTime = MyTM::my_mktime(datetime, m_isUTC);
		}
		/**
		* @param[in] unixTime=0
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		*/
		Datetime(const time_t &unixTime = 0, const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			validateInput(unixTime);
			m_unixTime = unixTime;
		}

		Datetime(const Datetime &original)
		{
			m_isUTC = original.m_isUTC;
			m_unixTime = original.m_unixTime;
		}
		/**
		* @param[in] year (1970 ~ 2999)
		* @param[in] mon (1 ~ 12)
		* @param[in] day (1 ~ 31)
		* @param[in] hour (0 ~ 23)
		* @param[in] min (0 ~ 59)
		* @param[in] sec (0 ~ 59)
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		* @details ex: 2021/1/1 13:00:00 UTC => EZ::Datetime(2021, 1, 1, 13, 0, 0, true);
		*/
		Datetime(const int &year, const int &mon, const int &day,
				 const int &hour, const int &min, const int &sec,
				 const bool &isUTC = false)
		{
			m_isUTC = isUTC;
			struct tm datetime;
			try
			{
				datetime.tm_year = year - DatetimeConstants::TM_BASE_YEAR;
				datetime.tm_mon = mon - DatetimeConstants::MONTH_OFFSET;
				datetime.tm_mday = day;
				datetime.tm_hour = hour;
				datetime.tm_min = min;
				datetime.tm_sec = sec;
			}
			catch (...)
			{
				throw DatetimeException("ERROR: Failed to parse input to struct tm");
			}
			validateInput(datetime);
			m_unixTime = MyTM::my_mktime(datetime, m_isUTC);
		}

		~Datetime()
		{
		}

		/**
		* 表示するタイムゾーンをUTCにするか現地時刻にするか指定する。\n
		* Set timezone of instance (UTC or loacl time). \n
		* isUTC = false : Express timestamp as localTime.\n
		* isUTC = true : Express timestamp as UTC.\n
		*/
		void setIsUTC(const bool &isUTC)
		{
			m_isUTC = isUTC;
		}
		/**
		* インスタンスに設定されたタイムゾーンがUTCならtrueを、現地時間ならfalseを返却する。 \n
		* Return true if the timezone of the instance is UTC. False if local time. \n
		* @details See also\n setIsUtc(const bool& isUTC = false)
		*/
		bool isUTC() const
		{
			return m_isUTC;
		}

		/**
		* タイムスタンプをstruct tmで返却する \n
		* Return the timestamp as a "struct tm". \n
		*/
		struct tm structTm() const
		{
			validateInput(m_unixTime);
			return MyTM::my_mkStructTm(m_unixTime, m_isUTC);
		}
		/**
		* タイムスタンプを文字列で返却する。出力文字列はデフォルトのフォーマットで返却される。 \n
		* Return the timestamp as a string. The output format is default format. \n
		* @returns strings of timestamp \n
		* ex: 2021/1/1 13:00:00 UTC
		*/
		std::string str() const
		{
			return m_parser.time2str(structTm());
		}
		/**
		* タイムスタンプを文字列で返却する。出力フォーマットは引数で指定する。 \n
		* Return the timestamp as a string. Output format is specified by argument. \n
		* @param[in] format	strings with format specifier \n
		* ex: format = "%Y/%m/%d_%H:%M:%S" => 2021/1/1_13:00:00
		*/
		std::string str(const std::string &format) const
		{
			return m_parser.time2str(structTm(), format);
		}
		/**
		* タイムスタンプを文字列で返却する。出力フォーマットは引数で指定する。 \n
		* Return the timestamp as a string. Output format is specified by argument. \n
		* @param[in] format	strings with format specifier\n
		* ex: format = "%Y/%m/%d_%H:%M:%S" => 2021/1/1_13:00:00
		*/
		std::string str(const char *format) const
		{
			return m_parser.time2str(structTm(), format);
		}

		/**
		* 日時を文字列で返却する。フォーマットは固定。 \n
		* Return the datetime as a string. Output format is fixed. \n
		* @details ex: 2021/1/1 13:00:00
		* @details This function is same as using str("%Y/%m/%d %H:%M:%S")
		*/
		std::string datetime() const
		{
			return str();
		}
		/**
		* 日付を文字列で返却する。フォーマットは固定。 \n
		* Return the date as a string. Output format is fixed. \n
		* @returns strings of timestamp
		* @details ex: 2021/1/1
		* @details This function is same as using str("%Y/%m/%d")
		*/
		std::string date() const
		{
			return str("%Y/%m/%d");
		}
		/**
		* 時刻を文字列で返却する。フォーマットは固定。 \n
		* Return the time as a string. Output format is fixed. \n
		* @returns strings of timestamp
		* @details ex: 13:00:00
		* @details This function is same as using str("%H:%M:%S")
		*/
		std::string timesOfday() const
		{
			return str("%H:%M:%S");
		}
		/**
		* 秒数を整数値で返却する \n
		* Return the number of seconds as an integer value.
		*/
		int sec() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_sec;
		}
		/**
		* 分値を整数値で返却する \n
		* Return the number of minutes as an integer value.
		*/
		int minute() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_min;
		}
		/**
		* 時刻値を整数値で返却する \n
		* Return the number of hours as an integer value.
		*/
		int hour() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_hour;
		}
		/**
		* 日にちを整数値で返却する \n
		* Return the number of day as an integer value.
		*/
		int day() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_mday;
		}
		/**
		* 月数を整数値で返却する \n
		* Return the number of month as an integer value.
		*/
		int month() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_mon + DatetimeConstants::MONTH_OFFSET;
		}
		/**
		* 西暦を整数値で返却する \n
		* Return the year as an integer value.
		*/
		long year() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_year + DatetimeConstants::TM_BASE_YEAR;
		}
		/**
		* 日時をUnix秒で返却する \n
		* Return the datetime in unix seconds.
		*/
		long long unixTime() const
		{
			return m_unixTime;
		}
		/**
		* 曜日を整数値で返却する \n
		* Return the day of the week as an integer value.
		* @returns 0 => Sunday, 1 => Monday, ..., 6 => Saturday
		*/
		int daysOfWeek() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_wday;
		}
		// TODO: ISO
		/**
		* 週番号を返却する \n
		* Return the week number as an integer value. \n
		*/
		// int weeksOfYear() const
		// {
		// 	auto tmpTm = structTm();
		// 	return (tmpTm.tm_yday / 7) + 1;
		// }

		/**
		* タイムゾーンを返却する \n
		* Return the timezone of the instance. \n
		* @details unix, mac: UTC, JST, EST, ...etc\n
		* @details Windows: UTC, 90min from UTC, -270 min from UTC, ...etc
		*/
		std::string timezone() const
		{
			auto tmpTm = structTm();
#if defined(_WIN32) || defined(_WIN64)
			TIME_ZONE_INFORMATION tzi;
			GetTimeZoneInformation(&tzi);
			std::stringstream ss;
			long bias = tzi.Bias;
			if (m_isUTC)
			{
				ss << "UTC";
			}
			else
			{
				ss << "(" << tzi.Bias << "min from UTC)";
			}
			return ss.str();
#else
			return std::string(tmpTm.tm_zone);
#endif
		}
		/**
		* サマータイムフラグを返却する \n
		* Return the summer time flag.
		* @returns number of isDst\n
		* @details 0 => Summer time is disabled.\n 1 => Summer time is enabled.\n negative => Depends on your computer. \n
		* Specification of return value is same as "struct tm". See also official documentation of struct tm.
		*/
		int isDst() const
		{
			auto tmpTm = structTm();
			return tmpTm.tm_isdst;
		}

		/**
		* コンピュータの現在時刻を取得する \n
		* Get the current time of the computer.
		* @param[in] isUTC=false	if true, UTC is set to timezone.\n if false, local time is applied.
		* @details ex: auto now = EZ::Datetime::currentTIme(true); 
		*/
		static Datetime now(const bool &isUTC = false)
		{
			time_t unixTime = time(NULL);
			return Datetime(unixTime, isUTC);
		}

		/**
		* 処理系で表現可能な最古の日付を返す \n
		* Returns the oldest date that can be handled with this library. 
		*/
		static Datetime minimum(const bool &isUTC = false)
		{
			return Datetime(time_t(DatetimeConstants::MINIMUM_SEC), isUTC);
		}

		/**
		* 処理系で表現可能な最新の日付を返す \n
		* Returns the newest date that can be handled with this library.
		*/
		//https://cpprefjp.github.io/reference/limits/numeric_limits/max.html
		static Datetime maximum(const bool &isUTC = false)
		{
			return Datetime(DatetimeConstants::MAXIMUM_SEC, isUTC);
		}

		/**
		* {年, 月, 日, 時, 分, 秒} の6つの要素をもつvectorを返却する \n
		* Returns a vector with 6 elements {year, month, day, hour, minute, second}.
		* @details ex: {2021,1,1,23,59,59}
		*/
		std::vector<int> toVector() const
		{
			auto tmpTm = structTm();
			return {
				tmpTm.tm_year + DatetimeConstants::TM_BASE_YEAR,
				tmpTm.tm_mon + DatetimeConstants::MONTH_OFFSET,
				tmpTm.tm_mday,
				tmpTm.tm_hour,
				tmpTm.tm_min,
				tmpTm.tm_sec};
		}

	private:
		/**
		* 入力値のチェックをする
		*/
		void validateInput(const time_t &time) const
		{
			if (time < DatetimeConstants::MINIMUM_SEC)
			{
				std::stringstream ss;
				ss << "Input time is out of range. minimum datetime is " << Datetime::minimum(m_isUTC).str() << std::endl;
				throw DatetimeException(ss.str());
			}

			if (time > DatetimeConstants::MAXIMUM_SEC)
			{
				std::stringstream ss;
				ss << "Input time is out of range. maximum datetime is " << Datetime::maximum(m_isUTC).str() << std::endl;
				throw DatetimeException(ss.str());
			}
		}
		/**
		* 入力値のチェックをする
		*/
		void validateInput(const struct tm &datetime) const
		{
			time_t tmp = MyTM::my_mktime(datetime, m_isUTC);
			validateInput(tmp);
		}
		/**
		* std::string を struct tm に変換する
		*/
		void setDateTime(const std::string &timestamp, const std::string &format)
		{
			struct tm tmpTm = m_parser.str2time(timestamp, format);
			try
			{
				m_unixTime = MyTM::my_mktime(tmpTm, m_isUTC);
				validateInput(m_unixTime);
			}
			catch (...)
			{
				std::stringstream ss;
				ss << "ERROR: "
				   << "\"" << timestamp << "\""
				   << " is in illegal time range." << std::endl;
				throw DatetimeException(ss.str());
			}
		}

	public:
		/**
		* ある日付に、指定の秒数を足した後の日付を返却する。 \n
		* Return the new Datetime object after adding the specified number of seconds to a Datetime object.
		*/
		Datetime operator+(const long long &right) const
		{
			struct tm time;
			time_t unixTimeR = time_t(right);
			time_t retSec = m_unixTime + unixTimeR;
			return Datetime(retSec, m_isUTC);
		}
		/**
		* ある日付に、TimeDeltaを足した後の日付を返却する。 \n
		* Return the new Datetime object after adding the specified TimeDelta object to a Datetime object.
		*/
		Datetime operator+(const TimeDelta &right) const
		{
			return operator+(right.totalSeconds());
		}

		Datetime operator-(const long long &right) const
		{
			return operator+(-right);
		}
		Datetime operator-(const TimeDelta &right) const
		{
			return operator+(-right.totalSeconds());
		}

		Datetime &operator=(const Datetime &right_t)
		{
			m_isUTC = right_t.m_isUTC;
			m_unixTime = right_t.m_unixTime;
			return *this;
		}
		/**
		* ある日付を、指定の秒数だけ進める \n
		* Add the specified number of seconds to a Datetime object.
		*/
		Datetime &operator+=(const long long &right)
		{
			time_t unixTimeR = time_t(right);
			m_unixTime += unixTimeR;
			return *this;
		}
		/**
		* ある日付をTimeDeltaだけ進める \n
		* Add the specified TimeDelta object to a Datetime object.
		*/
		Datetime &operator+=(const TimeDelta &right)
		{
			return operator+=(right.totalSeconds());
		}

		Datetime &operator-=(const long long &right)
		{
			return operator+=(-right);
		}
		Datetime &operator-=(const TimeDelta &right)
		{
			return operator+=(-right.totalSeconds());
		}
	};

	std::ostream &operator<<(std::ostream &stream, const Datetime &time)
	{
		stream << time.str();
		return stream;
	};

	/**
	* 2つの日付の時間差をTimeDelta型で返却する \n
	* Return the time difference between two Datetime objects as TimeDelta object.
	*/
	TimeDelta operator-(const Datetime &left, const Datetime &right)
	{
		time_t diffSec = left.unixTime() - right.unixTime();
		return TimeDelta(diffSec);
	}

	/**
	* 2つの日付のunixTimeが等しいか判定する。(タイムゾーンの差異は問わない) \n
	* Judge if the unix times on two Datetime objects are equal. (Difference in timezone does not matter)
	*/
	bool operator==(const Datetime &left, const Datetime &right)
	{
		auto diffSec = left - right;
		return diffSec == 0;
	}

	bool operator!=(const Datetime &left, const Datetime &right)
	{
		return !(left == right);
	}

	bool operator<(const Datetime &left, const Datetime &right)
	{
		auto diffSec = left - right;
		return diffSec < 0;
	}

	bool operator>(const Datetime &left, const Datetime &right)
	{
		auto diffSec = left - right;
		return diffSec > 0;
	}

	bool operator<=(const Datetime &left, const Datetime &right)
	{
		return (left < right) || (left == right);
	}

	bool operator>=(const Datetime &left, const Datetime &right)
	{
		return (left > right) || (left == right);
	}

}
#endif