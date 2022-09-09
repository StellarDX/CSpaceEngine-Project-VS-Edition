// Time Module for CSpaceEngine.

//#define NOMINMAX // Solve the conflict problem of windows.h
#include <sstream>
#include <cassert>
#include <ctime>
#include <cmath>
#include <format>
#include <windows.h> // Windows only
#include "..\headers\date\Date_Time.h"

_CSE_BEGIN

CSELog TimeLog;

CSEDateTime::CSEDateTime(const _TIME CSEDate& Date, const _TIME CSETime& Time, const TIME_ZONE_INFORMATION& TimeZone)
{
	OffsetSecs = -TimeZone.Bias * 60;
	_Date = Date;
	_Time = Time;
}

CSEDateTime::CSEDateTime(const _TIME CSEDate& Date, const _TIME CSETime& Time, const double& OffsetSec)
{
	OffsetSecs = OffsetSec;
	_Date = Date;
	_Time = Time;
}

CSEDateTime::CSEDateTime(const _TIME CSEDate& Date)
{
	_Date = Date;
	_Time = _TIME CSETime(0, 0, 0, 0); // Start of the day.
}

CSEDateTime CSEDateTime::AddDays(int ndays) const
{
	_TIME CSEDate DT = _Date;
	DT = DT.AddDays(ndays);
	return CSEDateTime(DT, _Time, OffsetSecs);
}

CSEDateTime CSEDateTime::AddMSecs(int msecs) const
{
	_TIME CSEDate DT = _Date;
	_TIME CSETime TM = _Time;
	int addDays = 0;
	TM = TM.AddMSecs(msecs, &addDays);
	DT = DT.AddDays(addDays);
	return CSEDateTime(DT, TM, OffsetSecs);
}

CSEDateTime CSEDateTime::AddMonths(int nmonths) const
{
	_TIME CSEDate DT = _Date;
	DT = DT.AddMonths(nmonths);
	return CSEDateTime(DT, _Time, OffsetSecs);
}

CSEDateTime CSEDateTime::AddSecs(int s) const
{
	_TIME CSEDate DT = _Date;
	_TIME CSETime TM = _Time;
	int addDays = 0;
	TM = TM.AddSecs(s, &addDays);
	DT = DT.AddDays(addDays);
	return CSEDateTime(DT, TM, OffsetSecs);
}

CSEDateTime CSEDateTime::AddYears(int nyears) const
{
	_TIME CSEDate DT = _Date;
	DT = DT.AddYears(nyears);
	return CSEDateTime(DT, _Time, OffsetSecs);
}

bool CSEDateTime::IsNull() const
{
	return _Date.IsNull() && _Time.IsNull();
}

bool CSEDateTime::IsValid() const
{
	return !IsNull() && _Date.IsValid() && _Time.IsValid();
}

float64 CSEDateTime::OffsetFromUTC() const
{
	return OffsetSecs;
}

void CSEDateTime::SetDate(const _TIME CSEDate& date)
{
	_Date = date;
}

void CSEDateTime::SetTime(const _TIME CSETime& time)
{
	_Time = time;
}

void CSEDateTime::SetOffsetFromUTC(int OffsetSeconds)
{
	OffsetSecs = OffsetSeconds;
}

void CSEDateTime::setTimeZone(const LONG& ToZone)
{
	OffsetSecs = ToZone * 3600;
}

CSEDateTime CSEDateTime::toUTC() const
{
	_TIME CSETime TM = _Time.AddMSecs((int)(-OffsetSecs * 1000.0));
	_TIME CSEDate DT = _Date.AddDays((int)ceil(-OffsetSecs * 1000 / 86400000.0));
	return CSEDateTime(DT, TM, 0);
}

CSEDateTime CSEDateTime::currentDateTime()
{
	SYSTEMTIME Time;
	TIME_ZONE_INFORMATION TimeZone;
	GetLocalTime(&Time);
	GetSystemTime(&TimeZone.StandardDate);
	GetTimeZoneInformation(&TimeZone);

	return CSEDateTime
	(
		_TIME CSEDate(Time.wYear, Time.wMonth, Time.wDay),
		_TIME CSETime(Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds),
		TimeZone
	);
}

CSEDateTime CSEDateTime::currentDateTimeUTC()
{
	SYSTEMTIME Time;
	GetSystemTime(&Time);
	return CSEDateTime
	(
		_TIME CSEDate(Time.wYear, Time.wMonth, Time.wDay),
		_TIME CSETime(Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds),
		0.0
	);
}

_TIME_BEGIN

class TimeException : public std::runtime_error // output errors
{
public:
	TimeException(const std::string& Error) : std::runtime_error{ Error }{}
};

//////////////////////////////////////////////////
// --------------------DATE-------------------- //
//////////////////////////////////////////////////

CSEDate::CSEDate(int y, int m, int d)
{
	years = y;
	months = m;
	days = d;
}

bool CSEDate::IsNull() const
{
	return years == -1 && months == -1 && days == -1;
}

bool CSEDate::IsValid() const
{
	if (IsNull()) { return false; }
	if (years == 1582 && months == 10 && days >= 5 && days <= 14) { return false; } // When Julian calendar had been replaced by Gregorian calendar...
	if ((months > 0 && months <= 12) && (days > 0 && days <= NumOfDaysInMonthInYear(months, years))) { return true; }
	else { return false; }
}

int CSEDate::year() const
{
	if (IsValid())
	{
		return years;
	}
	return -1;
}

int CSEDate::month() const
{
	if (IsValid())
	{
		return months;
	}
	return -1;
}

int CSEDate::day() const
{
	if (IsValid())
	{
		return days;
	}
	return -1;
}

CSEDate CSEDate::AddDays(int ndays)const
{
	int Y = years;
	int M = months;
	int D = days;

	if (ndays >= 0)
	{
		while (ndays > 0)
		{
			++D;
			if (D > NumOfDaysInMonthInYear(M, Y))
			{
				D -= NumOfDaysInMonthInYear(M, Y);
				++M;
			}

			if (M > 12)
			{
				M -= 12;
				++Y;
			}

			if (Y == 1582 && M == 10 && D == 5)
			{
				D = 15;
			}

			--ndays;
		}
	}

	else if (ndays < 0)
	{
		while (ndays < 0)
		{
			--D;
			if (D <= 0)
			{
				--M;
				D += NumOfDaysInMonthInYear(M, Y);
			}

			if (M <= 0)
			{
				M += 12;
				--Y;
			}

			if (Y == 1582 && M == 10 && D == 14)
			{
				D = 4;
			}

			++ndays;
		}
	}

	return CSEDate(Y, M, D);
}

CSEDate CSEDate::AddMonths(int nmonths)const
{
	int Y = years;
	int M = months;
	int D = days;

	M += nmonths;
	if (nmonths >= 0)
	{
		while (M > 12)
		{
			M -= 12;
			++Y;
		}
	}

	else if (nmonths < 0)
	{
		while (M <= 0)
		{
			M += 12;
			--Y;
		}
	}

	if (D > NumOfDaysInMonthInYear(M, Y))
	{
		D = NumOfDaysInMonthInYear(M, Y);
	}

	if (Y == 1582 && M == 10 && D > 4 && D < 15)
	{
		D = 15;
	}

	return CSEDate(Y, M, D);
}

CSEDate CSEDate::AddYears(int nyears)const
{
	int Y = years;
	int M = months;
	int D = days;

	Y += nyears;

	if (D > NumOfDaysInMonthInYear(M, Y))
	{
		D = NumOfDaysInMonthInYear(M, Y);
	}

	if (Y == 1582 && M == 10 && D > 4 && D < 15)
	{
		D = 15;
	}

	return CSEDate(Y, M, D);
}

void CSEDate::GetDate(int* year, int* month, int* day)const
{
	if (IsValid())
	{
		*year = years;
		*month = months;
		*day = days;
	}
	else
	{
		*year = 0;
		*month = 0;
		*day = 0;
	}
}

bool CSEDate::setDate(int year, int month, int day)
{
	if (IsValid(year, month, day))
	{
		years = year;
		months = month;
		days = day;
		return true;
	}
	else
	{
		throw TimeException("Invalid date");
		return false;
	}
}

std::string CSEDate::toString(std::string _Fmt)
{
	return std::vformat(_Fmt, std::make_format_args(years, months, days));
}

int CSEDate::dayOfWeek() const
{
	double JD = (double)toJulianDay();
	double d = fmod(JD + 1.5, 7);
	if (d < 0) { d += 7.0; }
	return lround(floor(d));
}

//int64 CSEDate::toJulianDay() const // Convert a Date class to julian day(without fraction).
//{
//	using namespace std;
//	// Reference: Jean Meeus. "Astronomical Algorithms-2nd Edition" Chapter 7, Page 60.
//	int Y = years;
//	int M = months;
//
//	if (M > 2) { /*do nothing...*/ }
//	else if (M == 1 || M == 2)
//	{
//		Y -= 1;
//		M += 12;
//	}
//
//	// Correct for the lost days in Oct 1582 when the Gregorian calendar replaced the Julian calendar.
//	int B = -2;
//	if (years > 1582 || years == 1582 && (months > 10 || (months == 10 && days >= 15)))
//	{
//		B = Y / 400 - Y / 100;
//	}
//
//	return floor(365.25 * Y) + floor(30.6001 * (M + 1)) + B + 1720997 + days;
//}

int64 CSEDate::toJulianDay() const // Convert a Date class to julian day(without fraction).
{
	/*
	 * This algorithm is taken from
	 * "Numerical Recipes in C, 2nd Ed." (1992), pp. 11-12
	 * and maybe converted to integer math in the future.
	*/
	static const long IGREG = 588829; // (15 + 31L * (10 + 12L * 1582))

	long ja, jy = years, jm;

	if (jy == 0){throw TimeException("julday: there is no year zero.");}

	if (jy < 0) { ++jy; }
	if (months > 2)
	{
		jm = months + 1;
	}
	else
	{
		--jy;
		jm = months + 13;
	}

	long Jul = (long)(floor(365.25 * jy) + floor(30.6001 * jm) + days + 1720995);

	if (days + 31L * (months + 12L * years) >= IGREG)
	{
		ja = jy / 100;
		Jul += 2 - ja + (ja / 4);
	}

	return Jul;
}

CSEDate CSEDate::currentDate()
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);
	return CSEDate(Time.wYear, Time.wMonth, Time.wDay);
}

CSEDate CSEDate::fromJulianDay(const double JD) // From Stellarium
{
	/*
	 * This algorithm is taken from
	 * "Numerical Recipes in C, 2nd Ed." (1992), pp. 14-15
	 * and converted to integer math.
	*/

	static const long JD_GREG_CAL = 2299161;
	static const int JB_MAX_WITHOUT_OVERFLOW = 107374182;
	const long julian = static_cast<long>(floor(JD + 0.5));

	long ta, jalpha, tb, tc, td, te;

	if (julian >= JD_GREG_CAL)
	{
		jalpha = (4 * (julian - 1867216) - 1) / 146097;
		ta = julian + 1 + jalpha - jalpha / 4;
	}
	else if (julian < 0)
	{
		ta = julian + 36525 * (1 - julian / 36525);
	}
	else
	{
		ta = julian;
	}

	tb = ta + 1524;
	if (tb <= JB_MAX_WITHOUT_OVERFLOW)
	{
		tc = (tb * 20 - 2442) / 7305;
	}
	else
	{
		tc = static_cast<long>((static_cast<unsigned long long>(tb) * 20 - 2442) / 7305);
	}
	td = 365 * tc + tc / 4;
	te = ((tb - td) * 10000) / 306001;

	int dd = tb - td - (306001 * te) / 10000;

	int mm = te - 1;
	if (mm > 12)
	{
		mm -= 12;
	}
	int yy = tc - 4715;
	if (mm > 2)
	{
		--(yy);
	}
	if (julian < 0)
	{
		yy -= 100 * (1 - julian / 36525);
	}

	return CSEDate(yy, mm, dd);
}

bool CSEDate::isLeap()
{
	if (years > 1582)
	{
		if (years % 100 == 0) { return (years % 400 == 0); }
		else { return (years % 4 == 0); }
	}
	else
	{
		return (years % 4 == 0); // astronomical year counting: strictly every 4th year.
	}
}

bool CSEDate::isLeap(int year)
{
	if (year > 1582)
	{
		if (year % 100 == 0) { return (year % 400 == 0); }
		else { return (year % 4 == 0); }
	}
	else
	{
		return (year % 4 == 0); // astronomical year counting: strictly every 4th year.
	}
}

bool CSEDate::IsValid(int year, int month, int day)
{
	if (year == 1582 && month == 10 && day >= 5 && day <= 14) { return false; } // When Julian calendar had been replaced by Gregorian calendar...
	if ((month > 0 && month <= 12) && (day > 0 && day <= NumOfDaysInMonthInYear(month, year))) { return true; }
	else { return false; }
}

//////////////////////////////////////////////////
// --------------------TIME-------------------- //
//////////////////////////////////////////////////

CSETime::CSETime(int h, int m, int s, int ms)
{
	hours = h;
	minutes = m;
	seconds = s;
	msecs = ms;
}

CSETime::CSETime(int h, int m, double s)
{
	hours = h;
	minutes = m;
	double sec;
	msecs = static_cast<int>(modf(s, &sec) * 1000);
	seconds = static_cast<int>(sec);
}

bool CSETime::IsNull() const
{
	return hours == -1 && minutes == -1 && seconds == -1 && msecs == -1;
}

bool CSETime::IsValid() const
{
	return !IsNull() && ((hours < 24 && minutes < 60 && seconds < 60 && msecs < 1000) && (hours >= 0 && minutes >= 0 && seconds >= 0 && msecs >= 0));
}

int CSETime::hour() const
{
	if (IsValid())
	{
		return hours;
	}
	return -1;
}

int CSETime::minute() const
{
	if (IsValid())
	{
		return minutes;
	}
	return -1;
}

int CSETime::second() const
{
	if (IsValid())
	{
		return seconds;
	}
	return -1;
}

int CSETime::msec() const
{
	if (IsValid())
	{
		return msecs;
	}
	return -1;
}

CSETime CSETime::AddMSecs(int ms, int* AddDays) const
{
	int H = hours;
	int M = minutes;
	int S = seconds;
	int mS = msecs;

	mS += ms;

	if (ms > 0)
	{
		while (mS >= 1000)
		{
			mS -= 1000;
			++S;

			if (S >= 60)
			{
				S -= 60;
				++M;
			}

			if (M >= 60)
			{
				M -= 60;
				++H;
			}

			if (H >= 24)
			{
				H -= 24;
				if (nullptr != AddDays)
				{
					++(*AddDays);
				}
			}
		}
	}

	if (ms < 0)
	{
		while (mS < 0)
		{
			mS += 1000;
			--S;

			if (S < 0)
			{
				S += 60;
				--M;
			}

			if (M < 0)
			{
				M += 60;
				--H;
			}

			if (H < 0)
			{
				H += 24;
				if (nullptr != AddDays)
				{
					--(*AddDays);
				}
			}
		}
	}

	return CSETime(H, M, S, mS);
}

CSETime CSETime::AddSecs(int s, int* AddDays) const
{
	int H = hours;
	int M = minutes;
	int S = seconds;
	int mS = msecs;

	S += s;

	if (s >= 0)
	{
		while (S >= 60)
		{
			S -= 60;
			++M;

			if (M >= 60)
			{
				M -= 60;
				++H;
			}

			if (H >= 24)
			{
				H -= 24;
				if (nullptr != AddDays)
				{
					++(*AddDays);
				}
			}
		}
	}

	if (s < 0)
	{
		while (S < 0)
		{
			S += 60;
			--M;

			if (M < 0)
			{
				M += 60;
				--H;
			}

			if (H < 0)
			{
				H += 24;
				if (nullptr != AddDays)
				{
					--(*AddDays);
				}
			}
		}
	}

	return CSETime(H, M, S, mS);
}

bool CSETime::setHMS(int h, int m, int s, int ms)
{
	if (IsValid(h, m, s, ms))
	{
		hours = h;
		minutes = m;
		seconds = s;
		msecs = ms;
		return true;
	}
	else
	{
		throw TimeException("Invalid Time");
		return false;
	}
}

std::string CSETime::toString(std::string _Fmt)
{
	return std::vformat(_Fmt, std::make_format_args(hours, minutes, seconds, msecs));
}

bool CSETime::IsValid(int h, int m, int s, int ms)
{
	return (h < 24 && m < 60 && s < 60 && ms < 1000) && (h >= 0 && m >= 0 && s >= 0 && ms > 0);
}

CSETime CSETime::currentTime()
{
	SYSTEMTIME Time;
	GetLocalTime(&Time);
	return CSETime(Time.wHour, Time.wMinute, Time.wSecond, Time.wMilliseconds);
}

_TIME_END

// Internal functions

CSEDateTime jdToDateTime(const double& jd, const TimeSpec timeSpec)
{
	assert((timeSpec == UTC) || (timeSpec == LocalTime));
	LONG Offset;
	TIME_ZONE_INFORMATION TimeZone;
	if (timeSpec == UTC) { Offset = 0; }
	else if (timeSpec == LocalTime)
	{
		GetSystemTime(&TimeZone.StandardDate);
		GetTimeZoneInformation(&TimeZone);
		Offset = TimeZone.Bias / (-60);
	}
	CSEDateTime result = CSEDateTime(_TIME CSEDate().fromJulianDay(jd), getTimeFromJulianDay(jd), TimeZone).AddSecs(Offset * 3600);
	if (!result.IsValid())
	{
		TimeLog.Out("DateTime", "ERROR", "Invalid DateTime.", SysLogLevel);
	}
	assert(result.IsValid());
	return result;
}

double GetJDFromSystem()
{
	return _TIME CSEDate::currentDate().toJulianDay() + TimeToJDFract(_TIME CSETime(0, 0, 0, 0).currentTime());
}

double GetJDFromBesEpoch(const double Epoch)
{
	return 2400000.5 + (15019.81352 + (Epoch - 1900.0) * 365.242198781);
}

double TimeToJDFract(const _TIME CSETime& Time)
{
	return static_cast<double>((Time.hour() - 12.0) / 24 + Time.minute() / 1440.0 + (Time.second() + Time.msec() / 1000.0) / 86400.0);
}

_TIME CSETime getTimeFromJulianDay(const double jd) // Stellarium function
{
	double decHours = std::fmod(jd + 0.5, 1.0) * 24.;
	int hours = int(std::floor(decHours));
	double decMins = (decHours - hours) * 60.;
	int mins = int(std::floor(decMins));
	double decSec = (decMins - mins) * 60.;
	int sec = int(std::floor(decSec));
	double decMsec = (decSec - sec) * 1000.;
	int ms = int(std::round(decMsec));

	if (ms >= 1000)
	{
		ms -= 1000;
		sec += 1;
	}
	if (sec >= 60)
	{
		sec -= 60;
		mins += 1;
	}
	if (mins >= 60)
	{
		mins -= 60;
		hours += 1;
	}
	if (hours >= 24)
	{
		TimeLog.Out("DateTime", "WARNING", "hours exceed a full day!", SysLogLevel);
	}
	hours %= 24;

	_TIME CSETime tm = _TIME CSETime(hours, mins, sec, ms);
	if (!tm.IsValid())
	{
		TimeLog.Out("DateTime", "WARNING", "Invalid Time:" + std::to_string(hours) + "/" + std::to_string(mins) + "/" + std::to_string(sec) + "/" + std::to_string(ms), SysLogLevel);
	}
	assert(tm.IsValid());
	return tm;
}

int NumOfDaysInMonthInYear(const int month, const int year) // From Stellarium
{
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		if (year > 1582)
		{
			if (year % 4 == 0)
			{
				if (year % 100 == 0)
				{
					return (year % 400 == 0 ? 29 : 28);
				}
				else
				{
					return 29;
				}
			}
			else
			{
				return 28;
			}
		}
		else
		{
			return (year % 4 == 0 ? 29 : 28);
		}
	case 0:
		return NumOfDaysInMonthInYear(12, year - 1);
	case 13:
		return NumOfDaysInMonthInYear(1, year + 1);
	default:
		break;
	}

	return 0;
}

_CSE_END
