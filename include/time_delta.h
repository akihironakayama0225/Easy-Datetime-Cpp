#ifndef _MY_TIME_DELTA_
#define _MY_TIME_DELTA_

#include <string>
#include <sstream>
#include <vector>
#include <cmath>

#include "datetime_exceptions.h"
#include "datetime_constants.h"

namespace EZ
{
    /**
	* @brief TimeDelta object
	* @details A timedelta object represents a duration, the difference between two dates or times.
	*/
    class TimeDelta
    {
        // 累計秒

        long long m_totalSeconds = 0;
        long long m_totalDays = 0;

        // 累計秒を分解したもの
        long long m_seconds = 0;
        long long m_minutes = 0;
        long long m_hours = 0;
        // long long m_days = 0;
        // 月の日数は一定でない。年の日数も一定でない（閏年があるため）
        // long long m_months = 0;
        // long long m_years = 0;

    public:
        TimeDelta()
        {
        }

        /**
        * 時刻差を累積秒数で設定する \n
        * Set the time difference in seconds. 
		* @param[in] totalSeconds Time difference expressed with seconds.
		* @details ex: 366days and 5mins => EZ::TimeDelta(31622700);
		*/
        TimeDelta(const long long &totalSeconds)
            : m_totalSeconds(totalSeconds)
        {
            parseTotalSeconds();
        }

        TimeDelta(const TimeDelta &origin)
        {
            m_totalSeconds = origin.totalSeconds();
            parseTotalSeconds();
        }

        /** 
        * 時刻差を {日数, 時間数, 分数, 秒数} の順に指定する \n
        * Set the time difference in the order of {days, hours, minutes, seconds}.
		* @param[in] days
		* @param[in] hours
		* @param[in] minutes
		* @param[in] seconds
		* @details ex: 366days and 5mins => EZ::TimeDelta(366, 0, 5, 0);
        * @details See also member function set().
		*/
        TimeDelta(const long long &days, const long long &hours, const long long &minutes, const long long &seconds)
        {
            set(days, hours, minutes, seconds);
        }

        ~TimeDelta()
        {
        }

        /**
		* 時刻差をセットする(日数, 時間数, 分の数, 秒数)  \n
        * Set the time difference in the order of (days, hours, minutes, seconds).
        * @param[in] days
		* @param[in] hours
		* @param[in] minutes
		* @param[in] seconds
        * @details ex: -3days, -4hours, 5mins => set(-3, -4, 5, 0)
		*/
        void set(const long long &days, const long long &hours, const long long &minutes, const long long &seconds)
        {
            m_totalSeconds = ((days * 24 + hours) * 60 + minutes) * 60 + seconds;
            parseTotalSeconds();
        }
        /**
		* 時刻差を秒数で返す \n
        * Return the time difference in seconds.
		*/
        long long totalSeconds() const
        {
            return m_totalSeconds;
        }
        /**
		* 時刻差を分で返す(1分未満は切り捨て) \n
        * Return the time difference in minutes. (Truncate less than 1 minute)
		*/
        long long totalMinutes() const
        {
            return m_totalSeconds / 60;
        }
        /**
		* 時刻差を時間数で返す(1時間未満は切り捨て) \n
        * Return the time difference in hours. (Truncate less than 1 hour)
		*/
        long long totalHours() const
        {
            return m_totalSeconds / 3600;
        }
        /**
		* 時刻差を日数で返す(1日未満は切り捨て) \n
        * Return the time difference in days. (Truncate less than 1 day)
		*/
        long long totalDays() const
        {
            return m_totalDays;
        }
        /**
		* 時刻差を週数で返す(7日未満は切り捨て) \n
        * * Return the time difference in weeks. (Truncate less than 7 days)
		*/
        long long totalWeeks() const
        {
            return m_totalDays / 7;
        }
        /**
		* 時刻差を[日, 時, 分, 秒]の4つの要素をもつvectorとして返却する \n
        * Return the time difference as a vector with four elements {day, hour, minute, second}.
        * @details ex: {3, 4, 5, 0} => 3days, 4hours, 5mins
		*/
        std::vector<long long> toVector() const
        {
            return {m_totalDays, m_hours, m_minutes, m_seconds};
        }
        /**
		* 時刻差を以下のフォーマットの文字列で返却する \n
        * Return the time difference as a string such as the following format.
        * @details ex: TimeDelta(days=3, hours=4, minutes=5, seconds=0)
		*/
        std::string str() const
        {
            std::stringstream ss;
            ss << "TimeDelta("
               << "days=" << m_totalDays
               << ", hours=" << m_hours
               << ", minutes=" << m_minutes
               << ", seconds=" << m_seconds
               << ")";
            return ss.str();
        }

        TimeDelta operator+() const
        {
            return TimeDelta(m_totalSeconds);
        }
        TimeDelta operator-() const
        {
            return TimeDelta(-m_totalSeconds);
        }
        /**
		* 2つのTimeDeltaを足し合わせる \n
        * Add two TimeDelta objects.
		*/
        TimeDelta operator+(const TimeDelta &right) const
        {
            return m_totalSeconds + right.totalSeconds();
        }
        /**
		* TimeDeltaと整数秒を足し合わせる \n
        * Add TimeDelta object and integer seconds.
		*/
        TimeDelta operator+(const long long &right) const
        {
            long long retSeconds = m_totalSeconds + right;
            return TimeDelta(retSeconds);
        }

        TimeDelta operator-(const TimeDelta &right) const
        {
            return operator+(-right.totalSeconds());
        }
        TimeDelta operator-(const long long &right) const
        {
            return operator+(-right);
        }
        /**
		* TimeDeltaをk倍する \n
        * Multiply TimeDelta by double value.
		*/
        TimeDelta operator*(const double &right) const
        {
            long long retSeconds = double(m_totalSeconds) * right;
            return TimeDelta(retSeconds);
        }
        /**
		* TimeDeltaどうしの商を計算する \n
        * Divide TimeDelta object by TimeDelta object.
		*/
        double operator/(const TimeDelta &right) const
        {
            if (std::abs(right.totalSeconds()) < DatetimeConstants::DOUBLE_EPSILON)
            {
                throw DatetimeException("ERROR: Could not divide by nearly ZERO value.");
            }
            return double(m_totalSeconds) / right.totalSeconds();
        }
        /**
		* TimeDeltaを1/k倍する \n
        * Divide TimeDelta object by double value.
		*/
        TimeDelta operator/(const double &right) const
        {
            if (std::abs(right) < DatetimeConstants::DOUBLE_EPSILON)
            {
                throw DatetimeException("ERROR: Could not divide by nearly ZERO value.");
            }
            long long retSeconds = double(m_totalSeconds) / right;
            return TimeDelta(retSeconds);
        }

        TimeDelta &operator+=(const long long &right)
        {
            m_totalSeconds += right;
            parseTotalSeconds();
            return *this;
        }
        TimeDelta &operator+=(const TimeDelta &right)
        {
            return operator+=(right.totalSeconds());
        }

        TimeDelta &operator-=(const long long &right)
        {
            return operator+=(-right);
        }
        TimeDelta &operator-=(const TimeDelta &right)
        {
            return operator+=(-right.totalSeconds());
        }

        TimeDelta &operator*=(const double &right)
        {
            m_totalSeconds = double(m_totalSeconds) * right;
            parseTotalSeconds();
            return *this;
        }

        TimeDelta &operator/=(const double &right)
        {
            return operator*=(1.0 / right);
        }

    private:
        void parseTotalSeconds()
        {
            long long remainedSecondsOfDay = m_totalSeconds % (24 * 3600);
            m_hours = remainedSecondsOfDay / 3600;
            m_minutes = remainedSecondsOfDay % 3600 / 60;
            m_seconds = remainedSecondsOfDay % 3600 % 60;
            m_totalDays = m_totalSeconds / (24 * 3600);
        }
    };

    /**
    * 2つのTimeDeltaが等しいか判定する \n
    * Return true if two TimeDelta objects are equal.
    */
    bool operator==(const TimeDelta &left, const TimeDelta &right)
    {
        return left.totalSeconds() == right.totalSeconds();
    }

    bool operator==(const TimeDelta &left, const long long &right)
    {
        return left.totalSeconds() == right;
    }

    bool operator==(const long long &left, const TimeDelta &right)
    {
        return left == right.totalSeconds();
    }

    bool operator!=(const TimeDelta &left, const TimeDelta &right)
    {
        return !operator==(left, right);
    }

    bool operator!=(const TimeDelta &left, const long long &right)
    {
        return !operator==(left, right);
    }

    bool operator!=(const long long &left, const TimeDelta &right)
    {
        return !operator==(left, right);
    }

    bool operator>(const TimeDelta &left, const long long &right)
    {
        return left.totalSeconds() > right;
    }

    bool operator>(const long long &left, const TimeDelta &right)
    {
        return left > right.totalSeconds();
    }

    bool operator<(const TimeDelta &left, const long long &right)
    {
        return left.totalSeconds() < right;
    }

    bool operator<(const long long &left, const TimeDelta &right)
    {
        return left < right.totalSeconds();
    }

    bool operator>=(const TimeDelta &left, const long long &right)
    {
        return left == right || left > right;
    }

    bool operator>=(const long long &left, const TimeDelta &right)
    {
        return left == right || left > right;
    }

    bool operator<=(const TimeDelta &left, const long long &right)
    {
        return left == right || left < right;
    }

    bool operator<=(const long long &left, const TimeDelta &right)
    {
        return left == right || left < right;
    }

    std::ostream &operator<<(std::ostream &stream, const TimeDelta &timeDelta)
    {
        stream << timeDelta.str();
        return stream;
    }

    const TimeDelta operator*(const double &left, const TimeDelta right)
    {
        return TimeDelta(double(right.totalSeconds()) * left);
    }
}
#endif