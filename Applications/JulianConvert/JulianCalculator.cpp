///////////////////////////////////////////////////////////
// A Simple Julian day calculator based on CSpaceEngine. //
///////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <CSE/Core.h>

using namespace std;
using namespace cse;

#define PROGSTART while(true) {
#define PROGEND }

// ----------Adjustable Variables---------- //
static const int Precision = 8;

// ----------Internal Variables---------- //
char ConvertFrom;

// ----------Functions---------- //

double ReducedJD(double JD)
{
    return JD - 2400000;
}

double ModifiedJD(double JD)
{
    return JD - 2400000.5;
}

int64 TruncatedJD(double JD)
{
    return (int64)cse::floor(JD - 2440000.5);
}

double DublinJD(double JD)
{
    return JD - 2415020;
}

double CNESJD(double JD) // https://en.wikipedia.org/wiki/CNES
{
    return JD - 2433282.5;
}

double CCSDSJD(double JD) // https://en.wikipedia.org/wiki/Consultative_Committee_for_Space_Data_Systems
{
    return JD - 2436204.5;
}

int64 LilianDate(double JD) // https://en.wikipedia.org/wiki/Lilian_date
{
    return (long long)cse::floor(JD - 2299159.5);
}

int64 RataDie(double JD) // https://en.wikipedia.org/wiki/Rata_Die
{
    return (int64)cse::floor(JD - 1721424.5);
}

double MarsSolDate(double JD) // https://en.wikipedia.org/wiki/Timekeeping_on_Mars#Mars_Sol_Date
{
    return (JD - 2405522) / 1.02749f;
}

double UnixTime(double JD) // https://en.wikipedia.org/wiki/Unix_time
{
    return (JD - 2440587.5) * 86400;
}

int64 dotNetDateTime(double JD) // https://en.wikipedia.org/wiki/.NET
{
    return (int64)(JD - 1721425.5) * 864000000000;
}

double BesselianYear(double JD)
{
    return 1900.0 + (JD - 2415020.31352) / 365.242198781;
}

// ----------Struct Functions---------- //

void Display(CSEDateTime GMT, float64 JD)
{
    cout << "/----------Result----------/" << '\n';
    if (ConvertFrom != '1'){cout << fixed << "GMT:            " << GMT.date().toString() << ' ' << GMT.time().toString() << '\n';}
    if (ConvertFrom != '2'){cout << fixed << setprecision(Precision) << "JD:             " << JD << '\n';}
    cout << fixed << setprecision(Precision) << "Reduced JD:     " << ReducedJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Modified JD:    " << ModifiedJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Truncated JD:   " << TruncatedJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Dublin JD:      " << DublinJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "CNES JD:        " << CNESJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "CCSDS JD:       " << CCSDSJD(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Lilian Date:    " << LilianDate(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Rata Die:       " << RataDie(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Mars Sol Date:  " << MarsSolDate(JD) << '\n';
    cout << fixed << setprecision(Precision) << "Unix time:      " << UnixTime(JD) << '\n';
    cout << fixed << setprecision(Precision) << ".NET DateTime:  " << dotNetDateTime(JD) << '\n';
    if (ConvertFrom != '3'){cout << fixed << setprecision(Precision) << "Besselian year: " << BesselianYear(JD) << '\n';}
}

bool GetCustomTime(CSEDateTime& DateTime)
{
    int Y, M, D, h, m; double s;
    cout << "Enter 6 numbers in sequence: Year, Month, Day, Hour, Minute, Second." << '\n';
    cin >> Y >> M >> D >> h >> m >> s;
    DateTime = CSEDateTime(epoch::CSEDate(Y, M, D), epoch::CSETime(h, m, s), 0.0);
    if (!DateTime.IsValid()){return false;}
    return true;
}

void ConvertFromGMT()
{
    cout << "Time source:" << '\n'
         << " 0 = System time" << '\n'
         << " 1 = Enter a time" << '\n';

    cout << "Default: 0" << '\n';

    char EnterTime;
    cin >> EnterTime;

    CSEDateTime DateTime;
    bool Valid = true;
    switch (EnterTime) // Step1: Get time
    {
    case '0':
        DateTime = CSEDateTime::currentDateTimeUTC();
        break;

    case '1':
        Valid = GetCustomTime(DateTime);
        break;

    default:
        DateTime = CSEDateTime::currentDateTimeUTC();
        break;
    }

    if (!Valid)
    {
        cerr << "Invalid time number.\n";
        return;
    }

    cout << "Base: [GMT]" << DateTime.date().toString() << ' ' << DateTime.time().toString() << '\n';
    float64 JD = DateTime.date().toJulianDay() + TimeToJDFract(DateTime.time());
    Display(DateTime, JD);
}

void GetDateTimeFromJD(float64 JD)
{
    Display(jdToDateTime(JD), JD);
}

void ConvertFromJD()
{
    float64 input;
    cout << "Julian Day Number: " << '\n';
    cin >> input;
    cout << fixed << "Base: [JD]" << input << '\n';
    GetDateTimeFromJD(input);
}

void ConvertFromB()
{
    float64 input;
    cout << "Besselian Year Number: " << '\n';
    cin >> input;
    cout << fixed << "Base: [B]" << input << '\n';
    GetDateTimeFromJD(GetJDFromBesEpoch(input));
}

void WikipediaLink()
{
    cout << "What do you want to know?" << '\n'
        << " 1 = Epoch" << '\n'
        << " 2 = Gregorian calendar" << '\n'
        << " 3 = Julian Day" << '\n'
        << " 4 = Besselian Year" << '\n'
        << " 0 = Back" << '\n';
    char LearnMore;
    cin >> LearnMore;

    switch (LearnMore)
    {
    case '1':
        system("start https://en.wikipedia.org/wiki/Epoch_(astronomy)");
        break;

    case '2':
        system("start https://en.wikipedia.org/wiki/Gregorian_calendar");
        break;

    case '3':
        system("start https://en.wikipedia.org/wiki/Julian_day");
        break;

    case '4':
        system("start https://en.wikipedia.org/wiki/Epoch_(astronomy)#Besselian_years");
        break;

    case '0':
        break;

    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////
// ------------------------------Main------------------------------ //
//////////////////////////////////////////////////////////////////////

int main()
{
    SetConsoleTitleW(CSE_TITLE_STRING);
	PROGSTART

    cout << '\n'
         << "|---------------------------------------------------|" << '\n'
         << "| Astronomy Epoch Calculator(Based on CSpaceEngine) |" << '\n'
         << "|---------------------------------------------------|" << '\n'
         << "Convert From:" << '\n'
         << " 1 = Gregorian calendar Date and GMT" << '\n'
         << " 2 = Julian Date" << '\n'
         << " 3 = Besselian Year" << '\n'
         //<< " M = More Features" << '\n'
         << " L = Learn More" << '\n'
         << " 0 = Exit" << '\n';

    cout << "Default: 1" << '\n';

    cin >> ConvertFrom;

    switch (ConvertFrom)
    {
    case '1':
        ConvertFromGMT();
        break;

    case '2':
        ConvertFromJD();
        break;

    case '3':
        ConvertFromB();
        break;

    case 'L':
    case 'l':
        WikipediaLink();
        break;

    case '0':
    case 'x':
        cout << "Exiting..." << '\n';
        exit(0);
        break;

    default:
        ConvertFromGMT();
        break;
    }

    PROGEND
}
