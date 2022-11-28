#include <iostream>
#include <fstream>
#include "CSE/SCStream.h"
#include "CSE/Object.h"

using namespace std;
using namespace cse;

int main()
{
	ISCStream is = ParseFile("MenuStar");
	ISCStream is2 = ParseFile("WhiteDwarf");
	ISCStream SolarSys = ParseFile("SolarSys");
	ISCStream PolarisSys = ParseFile("Polaris");

	Object MenuStar = GetSEObject(is, "Menu Star");
	Object MenuPlanet = GetSEObject(is, "Menu Planet 1");
	Object MenuMoon = GetSEObject(is, "Menu Moon 1.1");
	Object WDStar = GetSEObject(is2, "BA");
	Object Earth = GetSEObject(SolarSys, "Earth");
	Object Mars = GetSEObject(SolarSys, "Mars");
	Object Polaris = GetSEObject(PolarisSys, "HIP 11767 Aa");

	ofstream fout("TestReport_SCStream.sc");
	OSCStream os(fout);
	os.precision(10);
	os.encod(CP_UTF8);
	os << NoBooleans << MenuStar << MenuPlanet << MenuMoon;
	os << WDStar;
	os << Earth << Mars;
	os << Polaris;
	os.write();
}
