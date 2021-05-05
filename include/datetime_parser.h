#ifndef _MY_DATETIME_PARSER_
#define _MY_DATETIME_PARSER_

#include <string>
#include <regex>
#include <vector>
#include <time.h>
#include <sstream>
#include <iomanip>

#include "datetime_exceptions.h"
#include "datetime_constants.h"

// key valのペアからstruct_tmに正しく代入する
// struct_tm から文字列に正しくparseする

namespace EZ
{
    class MyParser
    {

    public:
        MyParser(){};
        ~MyParser(){};

        struct tm str2time(const std::string &timestamp, const std::string &format)
        {
            std::vector<std::string> keys;
            std::vector<int> values;
            std::vector<std::string> delimList;
            extractTimeKeyValues(keys, values, delimList, timestamp, format);

            std::string duplicated;
            if (duplicatedKeyExists(duplicated, keys))
            {
                std::stringstream ss;
                ss << "ERORR: Format specifier is duplicated."
                   << " \"" << duplicated << "\" "
                   << std::endl
                   << "in " << format << std::endl;
                throw DatetimeException(ss.str());
            }

            struct tm time;
            // Initialize tm
            time.tm_mon = 0;
            time.tm_mday = 1;
            time.tm_hour = 0;
            time.tm_min = 0;
            time.tm_sec = 0;

            // spec: Year must be specified.
            bool hasYearRegisterd = false;
            for (size_t idx = 0; idx < keys.size(); idx++)
            {
                if (keys[idx] == "%Y")
                {
                    if (values[idx] > 3000)
                    {
                        throw DatetimeException("Input year must be under 3001.");
                    }
                    if (values[idx] < 1970)
                    {
                        throw DatetimeException("Input year must be at least 1970.");
                    }
                    time.tm_year = values[idx] - DatetimeConstants::TM_BASE_YEAR;
                    hasYearRegisterd = true;
                    continue;
                }
                if (keys[idx] == "%m")
                {
                    time.tm_mon = values[idx] - DatetimeConstants::MONTH_OFFSET;
                    continue;
                }
                if (keys[idx] == "%d")
                {
                    time.tm_mday = values[idx];
                    continue;
                }
                if (keys[idx] == "%H")
                {
                    time.tm_hour = values[idx];
                    continue;
                }
                if (keys[idx] == "%M")
                {
                    time.tm_min = values[idx];
                    continue;
                }
                if (keys[idx] == "%S")
                {
                    time.tm_sec = values[idx];
                    continue;
                }
                else
                {
                    std::stringstream ess;
                    ess << "ERROR: "
                        << "\"" << keys[idx] << "\""
                        << " is invalid input specifier.";
                    throw DatetimeException(ess.str());
                }
            }
            if (!hasYearRegisterd)
            {
                throw DatetimeException("ERROR: Expression \"%Y\" (Year) must be designated.");
            }

            return time;
        }

        struct tm str2time(const std::string &timestamp)
        {
            return str2time(timestamp, DatetimeConstants::DEFAULT_INPUT_FORMAT);
        }

        std::string time2str(const struct tm &time, const std::string &format) const
        {
            std::vector<std::string> keys;
            std::vector<std::string> delimList;
            std::regex re2(R"(%[a-zA-Z])");
            getMatchedLists(keys, delimList, format, re2);

            std::stringstream ss;
            for (size_t idx = 0; idx < keys.size(); idx++)
            {
                ss << delimList[idx] << outValues(time, keys[idx]);
            }
            ss << delimList.back();
            return ss.str();
        }
        // DEFAULT
        std::string time2str(const struct tm &time) const
        {
            return time2str(time, DatetimeConstants::DEFAULT_OUTPUT_FORMAT);
        }

    private:
        std::string outValues(const struct tm &time, const std::string &key) const
        {
            std::stringstream ss;
            if (key == "%y")
            {
                ss << std::setw(4) << std::right << std::setfill('0') << time.tm_year + DatetimeConstants::TM_BASE_YEAR;
                return ss.str().substr(2);
            }
            if (key == "%Y")
            {
                ss << std::setw(4) << std::right << std::setfill('0') << time.tm_year + DatetimeConstants::TM_BASE_YEAR;
                return ss.str();
            }
            if (key == "%m")
            {
                ss << std::setw(2) << std::right << std::setfill('0') << time.tm_mon + DatetimeConstants::MONTH_OFFSET;
                return ss.str();
            }
            if (key == "%d")
            {
                ss << std::setw(2) << std::right << std::setfill('0') << time.tm_mday;
                return ss.str();
            }
            if (key == "%H")
            {
                ss << std::setw(2) << std::right << std::setfill('0') << time.tm_hour;
                return ss.str();
            }
            if (key == "%M")
            {
                ss << std::setw(2) << std::right << std::setfill('0') << time.tm_min;
                return ss.str();
            }
            if (key == "%S")
            {
                ss << std::setw(2) << std::right << std::setfill('0') << time.tm_sec;
                return ss.str();
            }
            if (key == "%Z")
            {
#if defined(_WIN32) || defined(_WIN64)
                TIME_ZONE_INFORMATION tzi;
                GetTimeZoneInformation(&tzi);
                std::stringstream ss;
                long bias = tzi.Bias;
                if (bias == 0)
                {
                    ss << "UTC";
                }
                else
                {
                    ss << "(" << tzi.Bias << "min from UTC)";
                }
                return ss.str().c_str();
#else
                ss << time.tm_zone;
#endif
                return ss.str();
            }

            std::stringstream ess;
            ess << "ERROR: "
                << "\"" << key << "\""
                << " is invalid output specifier.";
            throw DatetimeException(ess.str());
            return "";
        }

        void extractTimeKeyValues(
            std::vector<std::string> &keys, std::vector<int> &values, std::vector<std::string> &delimList,
            const std::string &timestamp, const std::string &format)
        {
            std::vector<std::string> delimListTime;
            std::vector<std::string> delimListFormat;

            // Parse format string
            std::regex re2(R"(%[a-zA-Z])");
            getMatchedLists(keys, delimListFormat, format, re2);

            // Parse timestamp string
            std::regex re(R"(\d+)");
            std::vector<std::string> result;
            getMatchedLists(result, delimListTime, timestamp, re);
            for (auto v : result)
            {
                values.push_back(my_stoi(v));
            }

            if (keys.size() != values.size())
            {
                std::stringstream ss;
                ss << "ERROR: mismatch format and timestamp!" << std::endl
                   << "timestamp: \t" << timestamp << std::endl
                   << "format: \t" << format << std::endl;
                throw DatetimeException(ss.str());
            }

            if (delimListFormat != delimListTime)
            {
                std::stringstream ss;
                ss << "ERROR: mismatch format and timestamp!" << std::endl
                   << "timestamp: \t" << timestamp << std::endl
                   << "format: \t" << format << std::endl;
                throw DatetimeException(ss.str());
            }
            delimList = delimListFormat;
        }

        void getMatchedLists(
            std::vector<std::string> &matches, std::vector<std::string> &delimList,
            const std::string &target, const std::regex re) const
        {
            std::vector<std::string>().swap(matches);
            std::vector<std::string>().swap(delimList);
            std::vector<int> startList;
            std::vector<int> endList;

            for (std::sregex_iterator it(std::begin(target), std::end(target), re), end; it != end; ++it)
            {
                auto &&m = *it;
                //std::cout << "position = " << m.position() << ", length = " << m.length() << ", str = '" << m.str() << '\'' << std::endl;
                matches.push_back(m.str());
                startList.push_back(m.position());
                endList.push_back(m.position() + m.length());
            }

            // for (int i = 0; i < startList.size(); i++){
            //     std::cout << startList[i] << " -> " << endList[i] << std::endl;
            // }
            //std::cout << "SST " << startList.size() << " " << endList.size() <<std::endl;
            if (startList.empty())
            {
                throw DatetimeException("ERROR: No specifier is contained.");
            }
            // First
            std::vector<int> startList2 = {0};
            std::vector<int> endList2 = {startList[0]};
            // Middle
            for (int i = 0; i < startList.size() - 1; i++)
            {
                startList2.push_back(endList[i]);
                endList2.push_back(startList[i + 1]);
            }
            // End
            startList2.push_back(endList[startList.size() - 1]);
            endList2.push_back(target.size());

            for (int i = 0; i < startList2.size(); i++)
            {
                delimList.push_back(target.substr(startList2[i], endList2[i] - startList2[i]));
                //std::cout << startList2[i] << " -> " << endList2[i] << std::endl;
            }

            // for (auto d : delimList){
            //     std::cout << d << ",";
            // }
            // std::cout << std::endl;
        }

        bool duplicatedKeyExists(std::string &duplicated, const std::vector<std::string> &formatList)
        {
            for (size_t idx = 0; idx < formatList.size() - 1; idx++)
            {
                for (size_t j = idx + 1; j < formatList.size(); j++)
                {
                    if (formatList[idx] == formatList[j])
                    {
                        duplicated = formatList[idx];
                        return true;
                    }
                }
            }
            return false;
        }

        // 文字列を正の数値に変換する（不正な文字列の場合エラーを返す)
        int my_stoi(const std::string &str)
        {
            std::smatch m;
            if (std::regex_search(str, m, std::regex("[^0-9]+")))
            {
                std::string strMsg = "Invalid string contains. str = '" + m.str() + "'";
                throw DatetimeException(strMsg);
            }
            return std::stoi(str);
        }

        // Not used now (文字列を区切り文字で分割する)
        std::vector<std::string> split(const std::string &str, const std::string &delim)
        {
            //https://marycore.jp/prog/cpp/std-regex-split-regex-token-iterator/
            std::vector<std::string> ret = {};

            std::regex regexDelim{delim}; // 分割用のセパレータ
            auto ite = std::sregex_token_iterator(str.begin(), str.end(), regexDelim, -1);
            auto end = std::sregex_token_iterator();
            while (ite != end)
            {
                ret.push_back(*ite++);
            }
            return ret;
        }
    };
}
#endif
