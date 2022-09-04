/////////////////////////////////////////////////////////////
//                                                         //
//         Time and Epoch module for CSpaceEngine          //
//                                                         //
// ------------------------------------------------------- //
// This file includes some general calendar functions      //
// Some of them taken from Stellarium 0.21.3(GPLv2 Lic.)   //
//                                                         //
// Time classes is similar to Qt's time class.             //
// But these are more simple to use.                       //
//                                                         //
/////////////////////////////////////////////////////////////

#pragma once

#ifndef _CSE_TIME_
#define _CSE_TIME_

#define _USE_CSE_DEFINES
#include "..\Core\CSECore.h"

#define NOMINMAX // Solve the conflict problem of windows.h
#include <Windows.h>

#define _TIME_BEGIN namespace epoch{
#define _TIME epoch::
#define _TIME_END }

/*
#if defined(__GNUG__)
#define LONG long
#endif
*/
_CSE_BEGIN

_TIME_BEGIN

class CSEDate
{
	int years = -1;         /*!< Years. All values are valid */
	int months = -1;        /*!< Months. Valid values : 1 (January) - 12 (December) */
	int days = -1;          /*!< Days. Valid values 1 - 28,29,30,31 Depends on month.*/

public:
	CSEDate(int y, int m, int d);
	CSEDate() {}

	bool IsNull() const;
	bool IsValid() const;

	int year() const;
	int month() const;
	int day() const;

	CSEDate AddDays(int ndays) const; // Returns a Date object containing a date ndays later than the date of this object (or earlier if ndays is negative).
	CSEDate AddMonths(int nmonths) const;
	CSEDate AddYears(int nyears) const;

	void GetDate(int* year, int* month, int* day) const; // Extracts the date's year, month, and day, and assigns them to *year, *month, and *day. The pointers may be null.
	bool setDate(int year, int month, int day);
	std::string toString(std::string _Fmt = "{}.{:02}.{:02}");

	/// <summary>
	/// Return a day number of week for date
	/// </summary>
	/// <returns>number of day: 0 - sunday, 1 - monday,..</returns>
	int dayOfWeek() const;

	/// <summary>
	/// Compute Julian day number from calendar date.
	/// Dates before 1582-10-15 are in the Julian Calendar.
	/// </summary>
	int64 toJulianDay() const;

	static CSEDate currentDate();

	/// <summary>
	/// Make from julianDay a year, month, day for the Julian Date julianDay represents.
	/// (This function is taken from Stellarium.)
	/// </summary>
	static CSEDate fromJulianDay(const double JD);

	/// <returns>true if year is a leap year. Observes 1582 switch from Julian to Gregorian Calendar.</returns>
	bool isLeap();
	static bool isLeap(int year);

	static bool IsValid(int year, int month, int day);
};

class CSETime
{
	int hours = -1;         /*!< Hours. Valid values 0 - 23. */
	int minutes = -1;       /*!< Minutes. Valid values 0 - 59. */
	int seconds = -1;       /*!< Seconds. Valid values 0 - 59 */
	int msecs = -1;         /*!< MilliSeconds. Valid values 0 - 999 */

public:
	CSETime(int h, int m, int s = 0, int ms = 0);
	CSETime(int h, int m, double s = 0);
	CSETime() {}

	bool IsNull() const;
	bool IsValid() const;

	int hour() const;
	int minute() const;
	int second() const;
	int msec() const;

	CSETime AddMSecs(int ms, int* AddDays = nullptr) const;
	CSETime AddSecs(int s, int* AddDays = nullptr) const;

	bool setHMS(int h, int m, int s, int ms = 0);
	std::string toString(std::string _Fmt = "{:02}:{:02}:{:02}.{:03}");

	static bool IsValid(int h, int m, int s, int ms = 0);
	static CSETime currentTime(); // Returns the current time as reported by the system clock.(Windows only)
};

_TIME_END

class CSEDateTime
{
	double OffsetSecs = 0;
	_TIME CSEDate _Date;
	_TIME CSETime _Time;

public:
	CSEDateTime(const _TIME CSEDate& Date, const _TIME CSETime& Time, const TIME_ZONE_INFORMATION& TimeZone); // Constructs a datetime with the given date and time, using the Time Zone
	CSEDateTime(const _TIME CSEDate& Date, const _TIME CSETime& Time, const double& OffsetSec);
	CSEDateTime(const _TIME CSEDate& Date);
	CSEDateTime() {} // Constructs a null datetime

	_TIME CSEDate date() const { return _Date; }
	_TIME CSETime time() const { return _Time; }
	LONG TimeZone() const { return (LONG)floor((OffsetSecs + 1800) / 3600.0); }

	CSEDateTime	AddDays(int ndays) const;
	CSEDateTime	AddMSecs(int msecs) const;
	CSEDateTime	AddMonths(int nmonths) const;
	CSEDateTime	AddSecs(int s) const;
	CSEDateTime	AddYears(int nyears) const;

	bool IsNull() const;
	bool IsValid() const;

	float64 OffsetFromUTC() const; // Returns this date-time's Offset From UTC in seconds.
	void SetDate(const _TIME CSEDate& date);
	void SetTime(const _TIME CSETime& time);
	void SetOffsetFromUTC(int OffsetSeconds);
	void setTimeZone(const LONG& ToZone);

	CSEDateTime toUTC() const;

	static CSEDateTime currentDateTime();
	static CSEDateTime currentDateTimeUTC();
};

// Internal functons

enum TimeSpec
{
	UTC,
	LocalTime
};
/// <summary>
/// Convert a Julian Day to CSE DateTime class
/// </summary>
/// <param name="jd">Julian Date</param>
/// <returns>CSE DateTime class</returns>
CSEDateTime jdToDateTime(const double& jd, const TimeSpec timeSpec = UTC);

/// <summary>
/// Get the current Julian Date from system time.
/// </summary>
/// <returns>the current Julian Date</returns>
double GetJDFromSystem();

/// <summary>
/// Get the Julian Day Number (JD) from Besselian epoch.(Taken from Stellarium)
/// </summary>
/// <param name="Epoch">epoch Besselian epoch, expressed as year</param>
/// <returns>Julian Day number (JD) for B(Year)</returns>
double GetJDFromBesEpoch(const double Epoch);

/// <summary>
/// Convert a time of day to the fraction of a Julian Day.
/// Note that a Julian Day starts at 12:00, not 0:00, and
/// so 12:00 == 0.0 and 0:00 == 0.5.
/// (This function is taken from Stellarium.)
/// </summary>
double TimeToJDFract(const _TIME CSETime& Time);

_TIME CSETime getTimeFromJulianDay(const double jd);

int NumOfDaysInMonthInYear(const int month, const int year); // From Stellarium

_CSE_END

#endif
