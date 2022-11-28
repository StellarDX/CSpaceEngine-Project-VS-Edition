#include <iostream>
#include <fstream>
#include "CSE/Core.h"

using namespace std;
using namespace cse;
using namespace cse::epoch;

int main()
{
	// A small test of datetime module.
	ofstream Report("TestReport_DateTime.txt");
	CSEDate Date0 = CSEDate(1582, 10, 15);
	CSEDate Date = CSEDate(2000, 1, 1);
	CSEDate Date2 = Date0.AddDays(-1);
	CSEDate Date3 = Date.AddMonths(-1);
	cout << Date2.toString() << '\n' << Date3.toString() << '\n';
	Report << Date2.toString() << '\n' << Date3.toString() << '\n';

	CSETime Time = CSETime(12, 0, 0, 0);
	CSETime Time2 = Time.AddMSecs(-1);
	cout << Time2.toString() << '\n';
	Report << Time2.toString() << '\n';

	CSEDateTime CurDateTime = CSEDateTime::currentDateTime();
	CSEDateTime DateTime(Date, Time, 0);
	CSEDateTime DateTime2 = DateTime.AddDays(31);
	cout << CurDateTime.toUTC().date().toString() << ' ' << CurDateTime.toUTC().time().toString() << '\n';;
	cout << DateTime2.date().toString() << ' ' << DateTime2.time().toString() << '\n';
	Report << CurDateTime.toUTC().date().toString() << ' ' << CurDateTime.toUTC().time().toString() << '\n';;
	Report << DateTime2.date().toString() << ' ' << DateTime2.time().toString() << '\n';
}
