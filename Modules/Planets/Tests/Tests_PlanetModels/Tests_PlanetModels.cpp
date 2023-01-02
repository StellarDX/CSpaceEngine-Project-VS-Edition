#include <iostream>
#include <fstream>
#include "CSE/Planets.h"

using namespace std;
using namespace cse;

int main()
{
    Object RockyPlanet = TerrestrialPlanetBase()(random);
    RockyPlanet.Name[0] = "RockyPlanet";
    Object GasGiant = GaseousPlanetBase()(random);
    GasGiant.Name[0] = "GasGiant";
    ofstream Report("TestReport_PlanetGen.sc");
    OSCStream fout(Report);
    fout << NoBooleans << RockyPlanet << GasGiant;
    fout.write();
}
