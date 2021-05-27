#pragma once
#include "gtest/gtest.h"
#include "datetime.h"

using namespace EZ;
class TestMyTimeZone : public ::testing::Test
{
protected:
    const static char *cTimeStamp;
    const static char *cFormat;
    static std::string sTimeStamp;
    static std::string sFormat;

    const static char *ccTimeStamp;
    static std::string ssTimeStamp;

    static std::vector<int> okVector;

    static void SetUpTestCase()
    {
        std::cout << "\tCALL SetUpTestCase()" << std::endl;
        cTimeStamp = "2021A10Bu5K13!!35%1";
        sTimeStamp = "2021A10Bu5K13!!35%1";
        cFormat = "%YA%dBu%mK%S!!%M%%H";
        sFormat = "%YA%dBu%mK%S!!%M%%H";

        ccTimeStamp = "2021/5/10 1:35:13";
        ssTimeStamp = "2021/5/10 1:35:13";
        okVector = {2021, 5, 10, 1, 35, 13};
    }

    static void TearDownTestCase()
    {
        std::cout << "\tCALL TearDownTestCase()" << std::endl;
    }

    virtual void SetUp()
    {
        //std::cout << "\t\tCALL SetUp()" << std::endl;
    }

    virtual void TearDown()
    {
        //std::cout << "\t\tCALL TearDown()" << std::endl;
    }
};

const char *TestMyTimeZone::cTimeStamp;
const char *TestMyTimeZone::cFormat;
std::string TestMyTimeZone::sTimeStamp;
std::string TestMyTimeZone::sFormat;

const char *TestMyTimeZone::ccTimeStamp;
std::string TestMyTimeZone::ssTimeStamp;

std::vector<int> TestMyTimeZone::okVector;

TEST_F(TestMyTimeZone, TimeStampAndFormat)
{
    std::vector<int> retVector;
    retVector = Datetime(cTimeStamp, cFormat).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, cFormat).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(cTimeStamp, sFormat).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, sFormat).toVector();
    EXPECT_EQ(retVector, okVector);

    retVector = Datetime(cTimeStamp, cFormat, false).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, cFormat, false).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(cTimeStamp, sFormat, false).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, sFormat, false).toVector();
    EXPECT_EQ(retVector, okVector);

    retVector = Datetime(cTimeStamp, cFormat, true).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, cFormat, true).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(cTimeStamp, sFormat, true).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(sTimeStamp, sFormat, true).toVector();
    EXPECT_EQ(retVector, okVector);
}

TEST_F(TestMyTimeZone, OnlyTimeStamp)
{
    std::vector<int> retVector;
    retVector = Datetime(ccTimeStamp).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(ssTimeStamp).toVector();
    EXPECT_EQ(retVector, okVector);

    retVector = Datetime(ccTimeStamp, false).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(ssTimeStamp, false).toVector();
    EXPECT_EQ(retVector, okVector);

    retVector = Datetime(ccTimeStamp, true).toVector();
    EXPECT_EQ(retVector, okVector);
    retVector = Datetime(ssTimeStamp, true).toVector();
    EXPECT_EQ(retVector, okVector);
}

TEST_F(TestMyTimeZone, StructTm)
{
    struct tm stm;
    stm.tm_year = 1990 - 1900;
    stm.tm_mon = 3 - 1;
    stm.tm_mday = 5;
    stm.tm_hour = 10;
    stm.tm_min = 26;
    stm.tm_sec = 15;

    auto timeA = Datetime(stm);
    auto timeLocal = Datetime(stm, false);
    auto timeUTC = Datetime(stm, true);

    EXPECT_EQ(636632775, timeUTC);
	auto a = timeLocal.timezone();
	auto b = timeUTC.timezone();
	std::cout << timeLocal.timezone() << " " << timeUTC.timezone() << std::endl;
    if (timeLocal.timezone() == "UTC" || timeLocal.timezone() == "GMT" || timeLocal.timezone() == "(0min from UTC)")
    {
        EXPECT_EQ(timeLocal.unixTime(), timeUTC.unixTime());
        EXPECT_EQ(timeLocal, timeUTC);
    }
    else
    {
        EXPECT_NE(timeLocal.unixTime(), timeUTC.unixTime());
        EXPECT_NE(timeLocal, timeUTC);
    }
    EXPECT_EQ(timeA, timeLocal);
    EXPECT_EQ(timeA.isUTC(), false);
}

TEST_F(TestMyTimeZone, TIME_T)
{
    time_t time = 636632775;
    auto timeA = Datetime(time);
    auto timeLocal = Datetime(time, false);
    auto timeUTC = Datetime(time, true);

    EXPECT_EQ(636632775, timeUTC);
    EXPECT_EQ("1990/03/05 10:26:15", timeUTC.str("%Y/%m/%d %H:%M:%S"));
    if (timeLocal.timezone() == "UTC" || timeLocal.timezone() == "GMT" || timeLocal.timezone() == "(0min from UTC)")
    {
        EXPECT_EQ(timeLocal.unixTime(), timeUTC.unixTime());
        EXPECT_EQ(timeLocal, timeUTC);
    }
    else
    {
        // TZが異なっても unixTimeが同じなら trueとしている
        
        EXPECT_EQ(timeLocal.unixTime(), timeUTC.unixTime());
        EXPECT_EQ(timeLocal, timeUTC);
        EXPECT_NE(timeLocal.hour(), timeUTC.hour());
    }

    if ("JST" == std::string(timeLocal.timezone()))
    {
        EXPECT_EQ(timeLocal.hour() - timeUTC.hour(), 9);
    }

    EXPECT_EQ(timeA, timeLocal);
    EXPECT_EQ(timeA.isUTC(), false);
}

TEST_F(TestMyTimeZone, NULL_ARG)
{
    auto timeA = Datetime();
    EXPECT_EQ(timeA, Datetime::minimum());
    EXPECT_EQ(timeA.isUTC(), false);
}

TEST_F(TestMyTimeZone, STATIC_FUNC)
{
    Datetime tmin0 = Datetime::minimum();
    Datetime tmin1 = Datetime::minimum(false);
    Datetime tmin2 = Datetime::minimum(true);

    EXPECT_EQ(tmin0, tmin1);
    EXPECT_EQ(tmin0, tmin2);
    EXPECT_EQ(tmin0.isUTC(), false);
    EXPECT_EQ(tmin1.isUTC(), false);
    EXPECT_EQ(tmin2.isUTC(), true);

    Datetime tcur0 = Datetime::now();
    Datetime tcur1 = Datetime::now(false);
    Datetime tcur2 = Datetime::now(true);

    EXPECT_EQ(tcur0, tcur1);
    EXPECT_EQ(tcur0, tcur2);
    EXPECT_EQ(tcur0.isUTC(), false);
    EXPECT_EQ(tcur1.isUTC(), false);
    EXPECT_EQ(tcur2.isUTC(), true);

    Datetime tmax0 = Datetime::now();
    Datetime tmax1 = Datetime::now(false);
    Datetime tmax2 = Datetime::now(true);

    EXPECT_EQ(tmax0, tmax1);
    EXPECT_EQ(tmax0, tmax2);
    EXPECT_EQ(tmax0.isUTC(), false);
    EXPECT_EQ(tmax1.isUTC(), false);
    EXPECT_EQ(tmax2.isUTC(), true);
}

TEST_F(TestMyTimeZone, TimeDeltaBetweenTimeZone)
{
    // if TZ
}

TEST_F(TestMyTimeZone, TimeStampAndFormatUTC)
{
    std::vector<int> retVector;
    Datetime tmp;

    tmp = Datetime(cTimeStamp, cFormat);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, cFormat);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(cTimeStamp, sFormat);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, sFormat);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(cTimeStamp, cFormat, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, cFormat, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(cTimeStamp, sFormat, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, sFormat, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(cTimeStamp, cFormat, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, cFormat, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(cTimeStamp, sFormat, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(sTimeStamp, sFormat, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);
}

TEST_F(TestMyTimeZone, OnlyTimeStampUTC)
{
    std::vector<int> retVector;
    Datetime tmp;

    tmp = Datetime(ccTimeStamp);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(ssTimeStamp);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(ccTimeStamp, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(ssTimeStamp, false);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), false);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(ccTimeStamp, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);

    tmp = Datetime(ssTimeStamp, true);
    retVector = tmp.toVector();
    EXPECT_EQ(tmp.isUTC(), true);
    EXPECT_EQ(retVector, okVector);
}

TEST_F(TestMyTimeZone, TimeZoneEquality)
{
    auto t11 = Datetime("2056/10/31 18:21:30");
    auto t12 = Datetime("2056/10/31 18:21:30", true);

    auto t21 = Datetime(2740209690);
    auto t22 = Datetime(2740209690, true);

    auto t31 = Datetime(2056, 10, 31, 18, 21, 30);
    auto t32 = Datetime(2056, 10, 31, 18, 21, 30, true);

    auto t41 = Datetime::now();
    auto t42 = Datetime::now(true);

    if (t11.timezone() != "UTC" && t11.timezone() != "GMT" && t11.timezone() != "(0min from UTC)"){
        EXPECT_NE(t11, t12);
        EXPECT_NE(t31, t32);
    }

    EXPECT_EQ(t21, t22);
    EXPECT_EQ(t41, t42);
}