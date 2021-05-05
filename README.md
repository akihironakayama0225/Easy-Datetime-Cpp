# Eazy Datetime (C++)
## [>> Switch to Japanese(日本語版)](https://qiita.com/drafts)


## Abstruct
- This is a library that allows you to easily calculate the datetime in C++.
- This is header-only library and supports multiple platforms such as Linux, mac and Windows.


## Introduction
Time calculation in C++ often using `struct tm`. However, struct tm has some parts that programmers have to pay attention to, for example year begins from 1900 and month begins from 0 month.

Therefore, I made a library wrapping struct tm and helping you to handle datetime easily.
 It is Named `Easy Datetime` and provide the following simple functions.

- Functions list
    - Datetime representation and calculation
    - Calculation of time delta between two datetimes
    - Mutual parsing between std::string and Datetime object
    - Getting the current time


## Contents
- [Installation](#installation)
- [EZ::Datetime](#ezdatetime)
    - [Getting the current time](#getting-the-current-time)
    - [Setting datetime](#setting-datetime)
    - [Format specifier](#format-specifier)
    - [Getting values from Datetime object](#getting-values-from-datetime-object)
    - [Subtraction between Datetimes](#Subtraction-between-datetimes)
- [EZ::TimeDelta](#eztimedelta)
    - [Setting the TimeDelta Object](#Setting-the-timedelta-object)
    - [Getting values from TimeDelta object](#getting-values-from-timedelta-object)
    - [Arithmetic operations of TimeDelta](#arithmetic-operations-of-timedelta)
- [Exception throwing](#exception-throwing)
- [Q&A](#qa)
- [Links](#links)


## Installation
- Add the `include` folder of this Git repository to your C++ project.
- Declare `#include "datetime.h"` as in the sample below and you're ready to use this library.


```C++:sample.cpp
/* Required */
#include "datetime.h"

/* Related headers */
#include <iostream>
#include <string>
#include <vector>

/* Namespace to use */
// using namespace EZ;

int main ()
{
	try {
		// ----------------- //
		// Write codes here. //
		// ----------------- //
	}
	catch (EZ::DatetimeException e) {
		// Show error message.
		std::cout << e.what() << std::endl;
	}

	return 0;
}

```


## EZ::Datetime
- This class handles datetime.
### Getting the current time
- You can get it by calling the static function `now()`. The type of return value is `EZ::Datetime`.
    - If you call `now(true)`, you can get the current time based on UTC.

```C++:sample.cpp
	// Getting the current time
	auto now = EZ::Datetime::now();
	std::cout << now << std::endl;

	/* Console output */
	// >> 2021/04/25 22:02:00 JST
```

### Setting datetime
- Setting datetime is done in the constructor. The ways to pass args to the constructor are as belows.
    - __1.__ Pass only the timestamp as an argument (__Note:__ In this case, the input format is interpreted as "%Y/%m/%d %H:%M:%S").
    - __2.__ Pass the timestamp and format as arguments.
    - __3.__ Pass integer values ​​in the order of year, month, day, hour, minute, second.

```C++:sample.cpp
	// example #1
	auto date1 = EZ::Datetime("2021/1/1 0:00:00" /*, false*/);
    // example #2
	auto date2 = EZ::Datetime("2021/1/1 0:00:00", "%Y/%m/%d %H:%M:%S" /*, false*/);
    // example #3
	auto date3 = EZ::Datetime(2022,1,1,0,0,0 /*, false*/);

	// Show timestamp using stream
	std::cout << "date1 is " << date1 << std::endl;
	std::cout << "date2 is " << date2 << std::endl;
	std::cout << "date3 is " << date3.str("Y=%Y m=%m d=%d H=%H M=%M S=%S TZ=%Z") << std::endl;

	/* Output */
	// >> date1 is 2021/01/01 00:00:00 JST
	// >> date2 is 2021/01/01 00:00:00 JST
	// >> date3 is Y=2022 m=01 d=01 H=00 M=00 S=00 TZ=JST
```

- If the last argument `"isUTC"` of the constructor is set to true, the timezone will be set to UTC.
    - If set to false, the time zone will be set to local time.
    - The default is false.

### Format specifier
- The Datetime class supports the following I / O specifiers.
    - Input format can be used in the `constructor`.
    - The output format can be used in the member function `str()`.

Specifier | Meaning | Input format | Output format |
|-|-|-|-|
|%Y | Specify the year with 4 digits | ○ Supported | ○ Supported |
|%y | Specify the year with the last two digits | __× Not supported__ | ○ Supported |
|%m | Specify month | ○ Supported (number of input digits: 1 to 2 digits) | ○ Supported (output with 2 digits) |
|%d | Specify day | ○ Supported (number of input digits: 1 to 2 digits) | ○ Supported (output with 2 digits) |
|%H | Specify hour | ○ Supported (number of input digits: 1 to 2 digits) | ○ Supported (output with 2 digits) |
|%M | Specify minutes | ○ Supported (number of input digits: 1 to 2 digits) | ○ Supported (output with 2 digits) |
|%S | Specify seconds | ○ Supported (number of input digits: 1 to 2 digits) | ○ Supported (output with 2 digits) |
|%Z | Specify time zone | __× Not supported__ (set by argument "isUTC") | ○ Supported |


### Getting values from Datetime object
- The following is a list of functions to get values.

```C++:sample.cpp
int 	sec()          // Return only "second".
int 	minute()       // Return only "minute".
int 	hour()         // Return only "hour".
int 	day()          // Return only "day".
int 	month()        // Return only "month".
long 	year()         // Return only "year".
int 	daysOfWeek()   // Return the day number starting on Sunday

// Returns the datetime as a vector with 6 elements {year, month, day, hour, minute, second}.
std::vector<long long> 	toVector ()
```

### Subtraction between Datetimes
- You can calculate the time difference between `EZ::Datetime` objects.
    - The type of return value is `EZ::TimeDelta`.

```C++:sample.cpp
	// example of subtraction
	auto delta = date2 - date1;
	std::cout << date2 << " - " << date1 << "\n\t = " << delta << std::endl;

	/* Output */
	// >> 2022/01/01 00:00:00 JST - 2021/01/01 00:00:00 JST
	// >>	 = TimeDelta(days=365, hours=0, minutes=0, seconds=0)
```


## EZ::TimeDelta
- This class handles the time difference between datetimes.
### Setting the TimeDelta Object
- The time difference is set in the constructor. The ways to pass the arguments are as follows.
    - __1.__ Pass the time difference in total seconds.
    - __2.__ Pass the time difference in the order of (day, hour, minute, second).

```C++:sample.cpp
	auto delta1 = EZ::TimeDelta(-365 * 24 * 3600);
	auto delta2 = EZ::TimeDelta(365, 0, 0, 0);
	
	std::cout << "delta1 = " << delta0 << std::endl;
	std::cout << "delta2 = " << delta1 << std::endl;

	/* Output */
	// >> delta1 = TimeDelta(days=-365, hours=0, minutes=0, seconds=0)
	// >> delta2 = TimeDelta(days=365, hours=0, minutes=0, seconds=0)
```

### Getting values from TimeDelta object
- There are two main ways to get values. One is to get the total values in the specified unit, like totalMinutes(), and the other is to get the std::vector consisted of (day, hour, minute, second).

```C++:sample.cpp
long long totalSeconds()  // Return the value in total seconds.
long long totalMinutes()  // Return the value in total minutes (truncate less than 1 minute).
long long totalHours()    // Return the value in total hours (truncate less than 1 hour).
long long totalDays()     // Return the value in total days (truncate less than 1 day).
long long totalWeeks()    // Return the value in total weeks (truncate less than 7 days).

// Return the std::vector consisted of (day, hour, minute, second).
std::vector<long long> 	toVector ()
```

### Arithmetic operations of TimeDelta
- __summation, subtraction__ : Can be performed between two TimeDelta objects.
- __multiplication, division__ : Can be performed with the TimeDelta object on the left and the double operand on the right.
    - You can also divide TimeDelta object by TimeDelta object, in this case the type of the return value is double.

```C++:sample.cpp
	// summation・subtraction
	std::cout << "delta0 + delta1 = "<< delta1 + delta1 << std::endl;
	std::cout << "delta0 - delta1 = "<< delta1 - delta1 << std::endl;
	// multiplication・division
	std::cout << "delta1 * 2.0 = "<< delta1 * 2 << std::endl;
	std::cout << "delta1 / 2.0 = "<< delta1 / 2 << std::endl;

	/* Output */
	// >> delta0 + delta1 = TimeDelta(days=730, hours=0, minutes=0, seconds=0)
	// >> delta0 - delta1 = TimeDelta(days=0, hours=0, minutes=0, seconds=0)
	// >> delta1 * 2.0 = TimeDelta(days=730, hours=0, minutes=0, seconds=0)
	// >> delta1 / 2.0 = TimeDelta(days=182, hours=12, minutes=0, seconds=0)
```

## Exception throwing
- Both Datetime object and TimeDelta object throw `EZ::DatetimeException`.
    - You can check the message of the thrown exception `ex` with calling `ex.what()`.

```C++:sample.cpp
	// Exception handling
	try{
		// Year is Negative.
		EZ::Datetime err = EZ::Datetime(-2000, 1, 1, 0, 0, 0);
	}
	catch(EZ::DatetimeException ex){
		std::cout << "Error message: " << ex.what() << std::endl;
	}

	/* Output */
	// >> Error message: Input time is out of range. minimum datetime is 1970/01/01 09:00:00 JST
```

## Q&A
<details><summary>Q1. Are there only two patterns of the timezone settings ?</summary><div>

- Yes. You can choose either UTC or local timezone.
- If you select the local timezone, location of that is automatically determined by your OS.
</div></details>

<details><summary>Q2. How do I set the timezone ?</summary><div>

- you can set it by the argument `const bool & isUTC` at the end of the Datetime constructor. Timezone settings can be set for each datetime object.
    - Local time, If isUTC = false.
    - UTC, if isUTC = true.
    - The default is isUTC = false.

</div></details>

<details><summary>Q3. How do I get the timezone information ?</summary><div>

- Call the member function `timezone()` on the Datetime instance.
    - The timezone information is returned as std::string.
</div></details>

<details><summary>Q4. Is this library able to handle fractional seconds ?</summary><div>

- It does not support fractional seconds. Fractional seconds are truncated and treated as integer seconds.
</div></details>

<details><summary>Q5. Are leap years and leap seconds taken into account ?</summary><div>

- Leap years are considered, but leap seconds is NOT considered.
    - The calculation of leap years and leap seconds is based on the calculation of `struct tm`, see also official documentation of struct tm.
</div></details>

<details><summary>Q6. How do I set summer time ?</summary><div>


- Similar to `struct tm`, daylight saving time is set automatically according to the timezone of your computer and cannot be set manually.
- You can check the status of daylight saving time of Datetime object by calling the member function `isDst()`.
    - isDst() => 0: Daylight saving time is inactivated.
    - isDst() => 1: Daylight saving time is activated.
    - isDst() => Negative value: Depends on your computer.

</div></details>

<details><summary>Q7. How long period can be handled with this library ?</summary><div>

- From 1970/1/1 0:00:00 UTC to 3000/1/2 0:00:00 UTC.
</div></details>

<details><summary>Q8. What is the essence of this library ?</summary><div>

- It is a wrapping library of `struct tm` and` time_t`.
</div></details>

<details><summary>Q9. Did you tested this library ?</summary><div>

- Yes, I wrote the test code in Google Test and executed it.
- If you find any bugs, please raise issues.
</div></details>



## Links
- Eazy datetime official docs. ((Under construction))

- Japanese documentation(日本語版)


#### Thank you for seeing this docs until the end. I would be happy if you could use this library.