#pragma once
#include "gtest/gtest.h"
#include "datetime.h"

using namespace EZ;
class TestDatetime : public ::testing::Test
{
protected:
    static Datetime time1;
    static Datetime time2;
    static Datetime time3;
    static long long diffSec;

    static void SetUpTestCase()
    {
        std::cout << "\tCALL SetUpTestCase()" << std::endl;
        time1 = Datetime();
        time2 = Datetime("2030/12/1 0:00:0");
        time3 = Datetime("2031/12/2 0:10:5");
        diffSec = 3600 * 24 * 365 * 1 + 3600 * 24 + 60 * 10 + 5;
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

Datetime TestDatetime::time1;
Datetime TestDatetime::time2;
Datetime TestDatetime::time3;
long long TestDatetime::diffSec;

TEST_F(TestDatetime, CalcDiffSeconds)
{
    EXPECT_EQ((time3 - time2), diffSec);
    EXPECT_EQ((time2 - time3), -diffSec);
    EXPECT_EQ(time2, (time2 + 0));
    EXPECT_EQ(time2, (time2 - 0));
    EXPECT_EQ(time3, (time2 + diffSec));
    EXPECT_EQ(time2, (time3 - diffSec));
}

TEST_F(TestDatetime, CopyOperator)
{
    Datetime tmp = Datetime();
    tmp = time2;
    ASSERT_EQ(tmp, time2);

    Datetime tmp2 = tmp + 5000;
    Datetime tmp3 = tmp - 5000;
    ASSERT_EQ(time2.str("%Y/%m/%d %H:%M:%S"), "2030/12/01 00:00:00");
    EXPECT_EQ(tmp, time2);
    EXPECT_GT(tmp2, tmp);
}

TEST_F(TestDatetime, ComparisonOperator)
{
    ASSERT_EQ(time2, time2);
    ASSERT_LT(time2, time3);

    EXPECT_TRUE((time2 == time2));
    EXPECT_FALSE((time2 != time2));
    EXPECT_FALSE((time2 < time2));
    EXPECT_FALSE((time2 > time2));
    EXPECT_TRUE((time2 >= time2));
    EXPECT_TRUE((time2 <= time2));

    EXPECT_FALSE((time2 == time3));
    EXPECT_TRUE((time2 != time3));

    EXPECT_FALSE((time2 > time3));
    EXPECT_FALSE(time2 >= time3);
    EXPECT_TRUE((time2 < time3));
    EXPECT_TRUE((time2 <= time3));
}

TEST_F(TestDatetime, ArithmeticOperator)
{
    auto tmpTime = Datetime("2021/3/6 10:30:25");
    auto a = tmpTime + TimeDelta(1000);
    auto b = tmpTime - TimeDelta(-1000);
    auto aa = tmpTime + 1000;

    EXPECT_EQ(tmpTime, Datetime("2021/3/6 10:30:25"));
    EXPECT_NE(tmpTime, a);
    EXPECT_EQ(a, b);
    EXPECT_EQ(a, aa);
}

TEST_F(TestDatetime, AssignmentOperator)
{
    auto tmpTime = Datetime("2021/3/6 10:30:25");
    tmpTime += TimeDelta(1000);
    ASSERT_NE(tmpTime, Datetime("2021/3/6 10:30:25"));
    tmpTime -= 1000;
    ASSERT_EQ(tmpTime, Datetime("2021/3/6 10:30:25"));
}

TEST_F(TestDatetime, ValidateConstruction)
{
    // Correct
    EXPECT_NO_THROW(Datetime("2020/10/9 23:30:15"));
    EXPECT_NO_THROW(Datetime("2020/1/1 3:3:3"));
    EXPECT_NO_THROW(Datetime("2020/01/01 03:03:03"));
    EXPECT_NO_THROW(Datetime("2020/12/31 23:59:59"));
    EXPECT_NO_THROW(Datetime("2038/12/31 23:59:59"));

    // Try to parse invalid timestamp to datetime.
    EXPECT_THROW(Datetime("2020-10-9 23:30:15"), DatetimeException);
    EXPECT_THROW(Datetime("2020/10/9 23,30:15"), DatetimeException);
    EXPECT_THROW(Datetime("2020/10s/9 23:30:15"), DatetimeException);
    EXPECT_THROW(Datetime("2020/109 23:30:15"), DatetimeException);
    EXPECT_THROW(Datetime("2020/-1/9 23:30:15"), DatetimeException);

    // Timestamp is out of range
    EXPECT_THROW(Datetime("1900/1/1 0:0:0"), DatetimeException);
    EXPECT_THROW(Datetime("2020/-1/9 23:30:15"), DatetimeException);
    EXPECT_THROW(Datetime("2020/13/1 0:0:0"), DatetimeException);
    EXPECT_THROW(Datetime("2020/1/1 0:0:60"), DatetimeException);
    EXPECT_THROW(Datetime("2020/1/1 0:60:00"), DatetimeException);
    EXPECT_THROW(Datetime("2020/12/9 24:0:00"), DatetimeException);
    EXPECT_THROW(Datetime("2020/12/32 25:30:60"), DatetimeException);
}

TEST_F(TestDatetime, OmittingSpecifier)
{
    EXPECT_THROW(Datetime("", ""), DatetimeException);
    EXPECT_THROW(Datetime("a", ""), DatetimeException);
    EXPECT_THROW(Datetime("a", "a"), DatetimeException);
    EXPECT_THROW(Datetime("", "a"), DatetimeException);
    EXPECT_THROW(Datetime("2020", ""), DatetimeException);
    EXPECT_THROW(Datetime("2020", "%S"), DatetimeException);
    EXPECT_THROW(Datetime("", "%Y/%m/d"), DatetimeException);
    EXPECT_THROW(Datetime(""), DatetimeException);
    EXPECT_THROW(Datetime("ab%"), DatetimeException);
    EXPECT_THROW(Datetime("2020"), DatetimeException);
    EXPECT_THROW(Datetime("_2020_", "%Y"), DatetimeException);
    EXPECT_THROW(Datetime("6", "%m"), DatetimeException);

    EXPECT_NO_THROW(Datetime("_2020_10_3", "_%Y_%m_%d"));
    EXPECT_THROW(Datetime("_2020_10_3", "%Y_%m_%d"), DatetimeException);
    EXPECT_THROW(Datetime("_2020_10_3", "_%Y_%m_%d_%K"), DatetimeException);
    EXPECT_THROW(Datetime("_2020_10_3", "_%Y_%m_%s"), DatetimeException);
    EXPECT_THROW(Datetime("_2020_10_3", "_%Y_%Y_%d"), DatetimeException);

    // Correct
    Datetime date;
    EXPECT_NO_THROW(date = Datetime("2020", "%Y"));
    std::vector<int> v = {2020, 1, 1, 0, 0, 0};
    std::vector<int> vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("2020_3", "%Y_%m"));
    v = {2020, 3, 1, 0, 0, 0};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("2020_3", "%Y_%d"));
    v = {2020, 1, 3, 0, 0, 0};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("2020_3", "%Y_%H"));
    v = {2020, 1, 1, 3, 0, 0};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("2020_3_", "%Y_%M_"));
    v = {2020, 1, 1, 0, 3, 0};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("-2020_3", "-%Y_%S"));
    v = {2020, 1, 1, 0, 0, 3};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_NO_THROW(date = Datetime("2020  10:11:12", "%Y  %H:%M:%S"));
    v = {2020, 1, 1, 10, 11, 12};
    vv = date.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
}

TEST_F(TestDatetime, ParsingFormat)
{
    // Try to parse invalid timestamp to datetime.
    EXPECT_NO_THROW(Datetime("2030/10/9 23:30:15", "%Y/%m/%d %H:%M:%S"));
    EXPECT_NO_THROW(Datetime("2030@10@9 23X30:#15", "%Y@%m@%d %HX%M:#%S"));
    EXPECT_THROW(Datetime("10/9 23:30:15", "%m/%d %H:%M:%S"), DatetimeException);
    EXPECT_THROW(Datetime("2030/10/9 23:30:15:40", "%Y/%m/%d %H:%M:%S:%S"), DatetimeException);
    EXPECT_THROW(Datetime("15/2030/10/9 23:30:15:40", "%M/%Y/%m/%d %H:%M:%S"), DatetimeException);
    EXPECT_THROW(Datetime("10/9 23:30:15:40", "%m/%d %H:%M:%S:%K"), DatetimeException);
}

TEST_F(TestDatetime, ValidateTimeRange)
{
    // Correct
    EXPECT_NO_THROW(Datetime("3000/1/2 0:0:0", true));
    EXPECT_THROW(Datetime("3000/1/2 0:0:01", true), DatetimeException);
    EXPECT_THROW(Datetime("1969/12/31 23:59:59", true), DatetimeException);
    EXPECT_NO_THROW(Datetime("1970/1/1 0:0:0", true));

    auto max = Datetime::maximum(true);
    auto min = Datetime::minimum(true);
    EXPECT_NO_THROW(auto a = Datetime(max));
    EXPECT_NO_THROW(Datetime(max.str("%Y/%m/%d %H:%M:%S"), true));
    EXPECT_NO_THROW(Datetime(max.structTm(), true));
    EXPECT_NO_THROW(Datetime(max.unixTime()));

    EXPECT_NO_THROW(auto b = Datetime(min));
    EXPECT_NO_THROW(Datetime(min.str("%Y/%m/%d %H:%M:%S"), true));
    EXPECT_NO_THROW(Datetime(min.structTm(), true));
    EXPECT_NO_THROW(Datetime(min.unixTime()));

    EXPECT_THROW(Datetime(max.unixTime() + 1), DatetimeException);
    EXPECT_THROW(Datetime(min.unixTime() - 1), DatetimeException);

    EXPECT_EQ(time1, min);
    EXPECT_THROW(time1 - 1, DatetimeException);
}

TEST_F(TestDatetime, InputEach_YmdHMS)
{
    auto t1 = Datetime(2021, 1, 1, 0, 0, 0);
    auto t11 = Datetime(2021, 1, 1, 0, 0, 0, false);
    auto t12 = Datetime(2021, 1, 1, 0, 0, 0, true);
    auto t2 = Datetime("2021/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    EXPECT_EQ(t1, t2);
    EXPECT_EQ(t1.isUTC(), false);
    EXPECT_EQ(t11.isUTC(), false);
    EXPECT_EQ(t12.isUTC(), true);

    auto t3 = Datetime(2040, 12, 31, 23, 59, 59);
    EXPECT_EQ(t3, Datetime("2040/12/31 23:59:59"));

    EXPECT_THROW(Datetime(2021, 0, 1, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(1969, 1, 1, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 2, 29, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 37, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, 25, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, -1, 3, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 13, 3, 0, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, -1, 0, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, 10, 60, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, 10, -1, 0), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, 10, 10, 60), DatetimeException);
    EXPECT_THROW(Datetime(2045, 10, 10, 10, 10, -1), DatetimeException);

    EXPECT_NO_THROW(Datetime(2045, 10, 24, 13, 0, 0));
}
