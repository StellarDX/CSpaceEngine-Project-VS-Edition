#define _USE_CSE_DEFINES

#include <iostream>
#include <fstream>
#include <sstream>
#include "./headers/CSE.h"

using namespace std;
using namespace cse;

int main()
{
  ISCStream is = ParseFile("DivineRPG.sc");
	//cout << is.Catalogs.size() << '\n';
	Object o = GetSEObject(is, "DivineStar");
	Object p = GetSEObject(is, "[DRPG2021] SSC9");

	ofstream fout("out.sc");
	ostringstream sout;
	OSCStream os(fout);
	//os.setf(sc::OSCManip::Fixed);
	//os.unsetf(sc::OSCManip::Booleans);
	os << Detail << Fixed << o << p;
	os.Write();
}
