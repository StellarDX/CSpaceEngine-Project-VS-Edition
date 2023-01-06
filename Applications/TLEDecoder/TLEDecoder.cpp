// Two line element set decoder based on CSE

#include "CSE/Planets/Orbit.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cse;

int main(int argc, char const* argv[])
{
	if (argc != 2)
	{ 
		cout << "Usage: TLEDecoder <Filename>\n";
		return -1;
	}
	
	ifstream fin(argv[1]);
	TLE TLEData(fin);
	fin.close();

	if (!TLEData.isValid()) { throw OrbitCalculateException("Invalid TLE Data"); }

	Object Spacecraft =
	{
		.Type = "Spacecraft",
		.Name = {TLEData.Title()},
		.ParentBody = "Earth",

		.Orbit = TLEData.Orbit()
	};

	ofstream fout(Spacecraft.Name[0] + ".sc");
	OSCStream SC(fout);
	SC.precision(15);
	SC.write();
	fout << "/* ---------- Spacecraft Basic Data ---------- *\\\n";
	TLEData.PrintSpacecraftData(fout);
	fout << "\\* ------------------------------------------- */\n\n";
	SC << NoWaterMarks << NoBooleans << Spacecraft;
	SC.write();
}
