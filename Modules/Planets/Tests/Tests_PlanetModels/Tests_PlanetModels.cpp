#include <iostream>
#include <fstream>
#include "CSE/Planets.h"

using namespace std;
using namespace cse;

int main()
{
    //Object RockyPlanet = TerrestrialPlanetBase()(random);
    Object RockyPlanet = RandomRockyPlanet();
    RockyPlanet.Name[0] = "RockyPlanet";
    //Object GasGiant = GaseousPlanetBase(0.0748)(random);
    Object GasGiant = RandomGasGiant();
    GasGiant.Name[0] = "GasGiant";
    ofstream Report("TestReport_PlanetGen.sc");
    OSCStream fout(Report);
    fout << NoBooleans << RockyPlanet << GasGiant;

    Object Sun =
    {
        .Type = "Star",
        .Name = {"Sun"},
        .ParentBody = "Solar System",
        .SpecClass = "G2V",
        .Mass = MassSol,
        .Dimensions = vec3(RadSol, RadSol - RadSol * 9E-6, RadSol) * 2.
    };

    Object Jupiter =
    {
        .Type = "Planet",
        .Name = {"Jupiter"},
        .ParentBody = "Sun",
        .Class = "GasGiant",
        .Mass = MassJupiter,
        .Dimensions = vec3(RadJupiter, RadJupiter - RadJupiter * 0.06487, RadJupiter) * 2.,

        .Orbit = 
        {
            .RefPlane = "Ecliptic",
            .PericenterDist = 4.9506 * AU,
            .Eccentricity = 0.0489,
            .Inclination = 1.303,
            .AscendingNode = 100.464,
            .ArgOfPericenter = 273.867,
            .MeanAnomaly = 20.020
        }
    };

    Object Ganymede =
    {
        .Type = "Moon",
        .Name = {"Ganymede"},
        .Mass = 1.4819E23,
        .Dimensions = vec3(2634100) * 2.,
    };

    MakeOrbit
    (
        &Jupiter,
        &Ganymede,
        &Sun,
        {
            .RefPlane = "Equator",
            .PericenterDist = 1069200000,
            .Eccentricity = 0.0013,
            .Inclination = 0.20
        }
    );

    Object Pluto =
    {
        .Type = "DwarfPlanet",
        .Name = {"Pluto"},
        .ParentBody = "Sun",
        .Class = "Aquaria",
        .AsterType = "Plutino",
        .Mass = 1.303E+22,
        .Dimensions = vec3(2376600, 2376600 - 2376600 * 0.01, 2376600),

        .Orbit =
        {
            .RefPlane = "Ecliptic",
            .PericenterDist = 29.658 * AU,
            .Eccentricity = 0.2488,
            .Inclination = 17.16,
            .AscendingNode = 110.299,
            .ArgOfPericenter = 113.834,
            .MeanAnomaly = 14.53
        }
    };

    Object Charon =
    {
        .Type = "Moon",
        .Name = {"Charon"},
        .Class = "Aquaria",
        .Mass = 1.586E+21,
        .Dimensions = vec3(606000, 606000 - 606000 * 0.005, 606000) * 2.
    };

    Object PlutoCharonBarycen = *MakeBinary
    (
        &Pluto,
        &Charon,
        &Sun,
        {
            .RefPlane = "Equator",
            .PericenterDist = 19587000,
            .Eccentricity = 0.0002,
            .Inclination = 0.080,
            .AscendingNode = 223.046
        }
    );

    fout << Sun << Jupiter << Ganymede << PlutoCharonBarycen << Pluto << Charon;
    fout.write();
    fout.clear();

    Report << '\n';

    Report << "// Rigid Roche limit result: " << RocheLimit(&Sun, &Jupiter, 0) << '\n'
        << "// Fluid Roche limit result: " << RocheLimit(&Sun, &Jupiter, 1) << '\n';

    Report << "// Hill Sphere result: " << HillSphere(&Sun, &Jupiter) << '\n';

    Report << "// Titus-Bode Law array: " << '[';
    for (size_t i = 0; i <= 7; i++)
    {
        Report << SolarSys_TBL[i];
        if (i < 7) { Report << ' '; }
    }
    Report << ']' << '\n';

    Report << "// Blagg array: " << '[';
    for (size_t i = 0; i <= 7; i++)
    {
        Report << SolarSys_BLF[i];
        if (i < 7) { Report << ' '; }
    }
    Report << ']' << '\n';

    Report << "// Dermott array: " << '[';
    for (size_t i = 0; i <= 4; i++)
    {
        Report << Jovian_DML[i];
        if (i < 4) { Report << ' '; }
    }
    Report << ']' << '\n';

    Report << "// Exponential array: " << '[';
    for (size_t i = 0; i <= 5; i++)
    {
        Report << Rho1Cnc_Exp[i];
        if (i < 5) { Report << ' '; }
    }
    Report << ']' << '\n';

    Report << '\n';

    string TLEString = "CSS\n1 48274U 21035A   23003.00000000  .00011693  00000-0  14603-3 0  9991\n2 48274  41.4762  82.6384 0005848 257.3417 110.2495 15.60092594 96015";
    TLE TLEData = TLE::ParseString(TLEString);
    Report << "/*" << TLEString << "*/" << '\n';
    Report << "// Valid: " << TLEData.isValid() << '\n';
    Report << "/* TLE Basic Data: \n";
    TLEData.PrintSpacecraftData(Report);
    Report << "*/\n";

    Object CSS =
    {
        .Type = "Spacecraft",
        .Name = {"Chinese Space Station"},
        .ParentBody = "Earth",

        .Orbit = TLEData.Orbit()
    };

    fout.precision(15);
    fout << NoWaterMarks << CSS;
    fout.write();
}
