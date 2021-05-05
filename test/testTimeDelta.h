#pragma once
#include "gtest/gtest.h"
#include "datetime.h"

using namespace EZ;
class TestTimeDelta : public ::testing::Test
{
protected:
    static TimeDelta delta1;
    static TimeDelta delta2;
    static TimeDelta delta3;
    static long long totalSeconds;
    static long long diffSecs;
    static long long diffMins;
    static long long diffHours;
    static long long diffDays;

    static void SetUpTestCase()
    {
        std::cout << "\tCALL SetUpTestCase()" << std::endl;
        diffSecs = 5;
        diffMins = 10;
        diffHours = 24;
        diffDays = 365;
        totalSeconds = 3600 * 24 * diffDays + 3600 * diffHours + 60 * diffMins + diffSecs;
        delta1 = TimeDelta();
        delta2 = TimeDelta(365, 0, 0, 0);
        delta3 = TimeDelta(totalSeconds);
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

TimeDelta TestTimeDelta::delta1;
TimeDelta TestTimeDelta::delta2;
TimeDelta TestTimeDelta::delta3;
long long TestTimeDelta::totalSeconds;
long long TestTimeDelta::diffSecs;
long long TestTimeDelta::diffMins;
long long TestTimeDelta::diffHours;
long long TestTimeDelta::diffDays;

TEST_F(TestTimeDelta, ParseTotalSeconds)
{
    EXPECT_EQ(31623005, delta3.totalSeconds());
    EXPECT_EQ(527050, delta3.totalMinutes());
    EXPECT_EQ(8784, delta3.totalHours());
    EXPECT_EQ(366, delta3.totalDays());
    EXPECT_EQ(366 / 7, delta3.totalWeeks());
    EXPECT_EQ(-527050, (-delta3).totalMinutes());
    EXPECT_EQ(-8784, (-delta3).totalHours());
    EXPECT_EQ(-366, (-delta3).totalDays());
    EXPECT_EQ(-366 / 7, (-delta3).totalWeeks());

    std::vector<long long> tmp = {diffDays + 1, 0, diffMins, diffSecs};
    std::vector<long long> tmp2 = delta3.toVector();
    EXPECT_EQ(true, MyHelper::isSame(tmp, tmp2));

    //std::cout << delta3 << std::endl;
}

TEST_F(TestTimeDelta, setTimeDelta)
{
    auto baseTime = Datetime("2000/1/1 0:0:0");
    auto delta = TimeDelta(0, 0, 0, 0);
    EXPECT_EQ(baseTime, baseTime + delta);
    delta.set(1, 0, 0, 0);
    EXPECT_EQ(Datetime("2000/1/2 0:0:0"), baseTime + delta);
    EXPECT_EQ(Datetime("1999/12/31 0:0:0"), baseTime - delta);
    delta.set(0, 1, 0, 0);
    EXPECT_EQ(Datetime("2000/1/1 1:0:0"), baseTime + delta);
    EXPECT_EQ(Datetime("1999/12/31 23:0:0"), baseTime - delta);
    delta.set(0, 0, 1, 0);
    EXPECT_EQ(Datetime("2000/1/1 0:1:0"), baseTime + delta);
    EXPECT_EQ(Datetime("1999/12/31 23:59:0"), baseTime - delta);
    delta.set(0, 0, 0, 1);
    EXPECT_EQ(Datetime("2000/1/1 0:0:1"), baseTime + delta);
    EXPECT_EQ(Datetime("1999/12/31 23:59:59"), baseTime - delta);
    delta.set(0, 0, 0, 60 * 10);
    EXPECT_EQ(Datetime("2000/1/1 0:10:0"), baseTime + delta);
    EXPECT_EQ(Datetime("1999/12/31 23:50:0"), baseTime - delta);
}

TEST_F(TestTimeDelta, ArithmeticOperator)
{
    EXPECT_EQ(2.0 * TimeDelta(0, 0, 3, 0), TimeDelta(0, 0, 6, 0));
    EXPECT_EQ(2.0 * TimeDelta(0, 0, 3, 0), TimeDelta(0, 0, 3, 0) * 2.0);
    EXPECT_EQ(2.0 * (TimeDelta(0, 0, 30, 0) + TimeDelta(0, 0, 30, 0)), TimeDelta(0, 2, 0, 0));
    EXPECT_EQ(2.0 * (TimeDelta(0, 0, -30, 0) + TimeDelta(0, 0, -30, 0)), TimeDelta(0, -2, 0, 0));
    EXPECT_EQ(TimeDelta(0, 10, 6, 0) - TimeDelta(0, 0, 6, 0), TimeDelta(0, 10, 0, 0));
    EXPECT_EQ(TimeDelta(0, 10, 6, 0) + TimeDelta(0, 0, 6, 0), TimeDelta(0, 10, 12, 0));

    auto delta = TimeDelta(0, 0, 3, 0);
    auto copyDelta = delta;

    std::vector<int> v = {0, 0, 6, 0};
    std::vector<long long> vv = (copyDelta + copyDelta).toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    v = {0, 0, 0, 0};
    vv = (copyDelta - copyDelta).toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    v = {0, 0, 6, 0};
    vv = (2.0 * copyDelta).toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    v = {0, 0, 6, 0};
    vv = (copyDelta * 2.0).toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    v = {0, 0, 1, 30};
    vv = (copyDelta / 2.0).toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));

    EXPECT_EQ(2.0, (copyDelta / (copyDelta / 2.0)));
}

TEST_F(TestTimeDelta, ArithmeticAssignmentOperator)
{
    auto baseTime = Datetime("2000/1/1 0:0:0");
    auto min = Datetime::minimum();

    auto delta = TimeDelta(0, 0, 3, 0);
    auto copyDelta = delta;
    EXPECT_EQ(Datetime("2000/1/1 0:30:0"), baseTime + delta * 10);
    EXPECT_EQ(Datetime("1999/12/31 23:30:0"), baseTime - delta * 10);
    EXPECT_EQ(Datetime("2000/1/1 0:1:30"), baseTime + delta / 2);
    EXPECT_EQ(Datetime("1999/12/31 23:58:30"), baseTime - delta / 2);
    EXPECT_EQ(Datetime("2000/1/1 20:0:0"), baseTime + delta * 20 * 20);
    EXPECT_EQ(Datetime("1999/12/31 4:00:0"), baseTime - delta * 20 * 20);
    ASSERT_EQ(copyDelta, delta);

    copyDelta += 300;
    std::vector<int> v = {0, 0, 8, 0};
    std::vector<long long> vv = copyDelta.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
    EXPECT_NE(copyDelta, delta);
    EXPECT_EQ(Datetime("2000/1/1 0:08:0"), baseTime + copyDelta);

    copyDelta *= 60;
    v = {0, 8, 0, 0};
    vv = copyDelta.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
    EXPECT_EQ(Datetime("2000/1/1 8:00:0"), baseTime + copyDelta);

    copyDelta /= 60;
    v = {0, 0, 8, 0};
    vv = copyDelta.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
    EXPECT_EQ(Datetime("2000/1/1 0:08:0"), baseTime + copyDelta);

    copyDelta -= 300;
    v = {0, 0, 3, 0};
    vv = copyDelta.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
    EXPECT_EQ(Datetime("2000/1/1 0:03:0"), baseTime + copyDelta);

    auto delta22 = TimeDelta(365 * 10, 5, 5, 5);
    v = {3650, 5, 5, 5};
    vv = delta22.toVector();
    EXPECT_EQ(true, MyHelper::isSame(v, vv));
}

TEST_F(TestTimeDelta, DevideByZero)
{
    TimeDelta delta(500);
    EXPECT_THROW(delta / 0, DatetimeException);
    EXPECT_THROW(delta / -0, DatetimeException);
    EXPECT_NO_THROW(delta / DatetimeConstants::DOUBLE_EPSILON);

    Datetime t = Datetime::now();
    EXPECT_THROW(t + (delta / -0), DatetimeException);
    EXPECT_THROW(t - (delta / -0), DatetimeException);
}

// 仕様: 閏年の計算はstruct tmに準拠（対応済み）
// 閏秒は考慮しない

TEST_F(TestTimeDelta, LeapYear)
{
    auto t1 = Datetime("2020/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    auto t2 = Datetime("2021/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    TimeDelta delta(366, 0, 0, 0);
    EXPECT_EQ(delta, t2 - t1);
    EXPECT_EQ(-delta, t1 - t2);

    auto t3 = Datetime("2032/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    auto t4 = Datetime("2033/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    EXPECT_EQ(+delta, t4 - t3);
    EXPECT_EQ(-delta, t3 - t4);

    auto t5 = Datetime("2099/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    auto t6 = Datetime("2101/1/1 0:0:00", "%Y/%m/%d %H:%M:%S");
    TimeDelta delta2(365 * 2, 0, 0, 0);
    EXPECT_EQ(+delta2, t6 - t5);
    EXPECT_EQ(-delta2, t5 - t6);
}

TEST_F(TestTimeDelta, set)
{
    auto delta = TimeDelta();
    delta.set(-3, -4, 5, 0);
    EXPECT_EQ(delta, TimeDelta(-273300));
    EXPECT_EQ(TimeDelta(-1, 0, 0, 24 * 3600), TimeDelta());
}
