// -*- coding: utf-8 -*-

#include <iostream>
#include <fstream>
#include <vector>

// AsterBeltCreator C++ Edition based on CSpaceEngine
// By StellarDX Astronomy.
// Original Python version by JackDole, 2016
// https://forum.spaceengine.org/viewtopic.php?f=3&t=114#p1126

// Default english strings is taken from python version
// Object and orbit generation will use "realistic" models from CSE Library
// Realistic style of names
// Add shape models instead of custom inclination and ascending node.
// Custom brighteness of comets

// This version works with Visual Studio 2022 with C++20 standard.

#include "CSE/Random.h"
#include "CSE/SCStream.h"
#include "CSE/Object.h"
#include "CSE/Planets.h"

using namespace std;
using namespace cse;

#define _WHILE_TRUE while(1){
#define _END_WHILE_TRUE }

#define MaxMoonNumber 5

string LngStr02 = "Type of object \n 1 = Asteroid belt \n 2 = Comet cluster \n 3 = Minor moons of a planet \n 0 = Exit \n Default = 1 : ";
string LngStr03 = "Number of objects \n Default = 50 : ";
string LngStr04 = "Name for group of objects \n Default = {} : ";
string LngStr05 = "Parent object \n Default = Sun : ";
string LngStr07 = "Total mass of the main objects in \033[1m\033[33msolar mass.\033[0m\n";
string LngStr08 = "If your object have \033[1m\033[36mEarth Mass\033[0m, \033[1mput a '-' sign for the number\033[0m\n";
string LngStr09 = "Example : Earth + Moon = 1 + 0.012302 = -1.012302\n";
string LngStr10 = "\033[1mUsed to calculate binary object max distance;\033[0m an approximate value is sufficient. : ";
string LngStr11 = "Minimum radius in \033[1m\033[33mAU\033[0m\n";
string LngStr12 = "If you have \033[1m\033[36mKm\033[0m, \033[1mput a '-' sign for the number\033[0m\n";
string LngStr14 = " Default = 5.0 AU : ";
string LngStr15 = "Maximum radius in \033[1m\033[33mAU\033[0m\n";
string LngStr16 = "If you have \033[1m\033[36mKm\033[0m, \033[1mput a '-' sign for the number\033[0m\n";
string LngStr17 = " Default = 6.0 AU : ";
string LngStr18 = "Minimum radius of objects in \033[1m\033[33mkm\033[0m \n Default = 0.1 : ";
string LngStr19 = "Maximum radius of objects in \033[1m\033[33mkm\033[0m \n Default = 50.0 : ";
string LngStr23 = "RefPlane (Ecliptic, Equator, Extrasolar) \n Default = Equator : ";
string LngStr24 = "Epoch \n Default = now : ";
string LngStr25 = "Output file name \n Default = {} : ";
string LngStr26 = "Inclination to the reference plane in degree \n Default = 0 : ";
string LngStr27 = "AscendingNode in degrees \n Default = 0 : ";
string LngStr31 = "Shape of Asteroid belt or rings\n 1 = Ring(Single plane) \n 2 = Sphere \n 3 = Torus(Shape like tire) \n Default = 3 : ";
string LngStr32 = "Enable realistic style of names (1 = enable) \n Default = 0 : ";
string LngStr33 = "Minimum absolute magnitude of comets \n Default = 5 : ";
string LngStr34 = "Maximum absolute magnitude of comets \n Default = -5 : ";

// SE style format strings
string SEStyleAster = "{} S{}";
string SEStyleComet = "{} C{}";
string SEStyleDMoon = "{} D{}";

// Realictic Strings(generate functions)
string RealAsterString(CSEDateTime DateTime, uint64 Number)
{
    // https://en.wikipedia.org/wiki/Provisional_designation_in_astronomy
    char HalfMonth = 'A' + (DateTime.date().month() - 1) * 2 +
        (DateTime.date().day() <= 15 ? 0 : 1);
    HalfMonth += HalfMonth >= 'I' ? 1 : 0;
    uint64 Index = (Number - 1) / 25;
    char Mod = 'A' + ((Number - 1) % 25);
    Mod += Mod >= 'I' ? 1 : 0;

    return "{} " + vformat("({}) {} {}{}{}", make_format_args(Number, DateTime.date().year(), HalfMonth, Mod, Index));
}

string RealCometString(CSEDateTime DateTime, uint64 Number, float64 CenterObjMass, float64 PericenterDist, float64 Eccentricity)
{
    char HalfMonth = 'A' + (DateTime.date().month() - 1) * 2 +
        (DateTime.date().day() <= 15 ? 0 : 1);
    HalfMonth += HalfMonth >= 'I' ? 1 : 0;
    char IsPeriodic;
    if (GetPeriod(CenterObjMass, PericenterDist, Eccentricity) < 200. * TropicalYear) { IsPeriodic = 'P'; }
    else { IsPeriodic = 'C'; }

    return "{} " + vformat("{}|{} {}{}", make_format_args(IsPeriodic, DateTime.date().year(), HalfMonth, Number));
}

string RealDMoonString(uint64 Number, int Year, string Type, string Parent, string Style, bool AddParent = true)
{
    auto ConvertToRoman = [](uint64 number) // fine up to 3999.
    {
        string thousands[] = { "", "M", "MM", "MMM" };
        string hundreds[] = { "", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM" };
        string tens[] = { "", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC" };
        string units[] = { "", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
        return thousands[number / 1000] + hundreds[(number % 1000) / 100] + tens[(number % 100) / 10] + units[number % 10];
    };

    if (Style == "Roman" && Number <= 3999) { return "{} " + ConvertToRoman(Number); }

    char IsRing;
    if (Type == "Ring") { IsRing = 'R'; }
    else { IsRing = 'S'; }

    if (!AddParent) { return vformat("{}|{} {} {}", make_format_args(IsRing, Year, Parent, Number)); }
    return "{} " + vformat("{}|{} {} {}", make_format_args(IsRing, Year, Parent, Number));
}

// ---------- Structs ---------- //

struct AsterBeltSettings
{
    // APPERENCE
    string Type;
    string Name;

    // Center Object
    string CenterObjectName;
    float64 CenterObjectMass;

    // Bastc Settings
    uint64 NParticles; // Number of objects created
    uint64 NBigBodies; // Number of Moon/DwarfPlanets created
    bool EnableRealNames;
    float64 MinRadius;
    float64 MaxRadius;
    float64 ProbMoon = 0.11;
    float64 ProbBinary = 0.3;
    float64 MinAbsMagn;
    float64 MaxAbsMagn;

    // ARRANGE
    float64 InnerRadius;
    float64 OuterRadius;
    float64 Inclination;
    float64 AscendingNode;
    string Shape;
    string RefPlane;
    float64 Epoch;

    string OutputFileName;
};

template<typename genType>
void GetInput(genType* Dst, genType Def, string arg = "")
{
    cout << arg;
    cin >> *Dst;
    if (cin.fail()){*Dst = Def;}
    cin.clear();
    cin.ignore();
    cin.sync();
}

void GetAllLine(string* Dst, string Def, string arg = "")
{
    cout << arg;
    getline(cin, *Dst);
    cin.clear();
    cin.ignore();
    cin.sync();
}

string NameStr(AsterBeltSettings Conf, Object Particle, size_t i)
{
    string NameString;
    if (Conf.EnableRealNames)
    {
        if (Conf.Type == "Asteroid")
        {
            NameString = vformat(RealAsterString
            (
                epoch::CSEDate::fromJulianDay(Conf.Epoch),
                i
            ), make_format_args(Conf.Name));
        }
        else if (Conf.Type == "Comet")
        {
            NameString = vformat(RealCometString
            (
                epoch::CSEDate::fromJulianDay(Conf.Epoch),
                i,
                Conf.CenterObjectMass,
                Particle.Orbit.PericenterDist,
                Particle.Orbit.Eccentricity
            ), make_format_args(Conf.Name));
        }
        else if (Conf.Type == "DwarfMoon")
        {
            string Style = "";
            if (Conf.Shape != "Ring") { Style = "Roman"; }
            NameString = vformat(RealDMoonString
            (
                i,
                epoch::CSEDate::fromJulianDay(Conf.Epoch).year(),
                Conf.Shape,
                Conf.CenterObjectName.substr(0, 1),
                Style
            ), make_format_args(Conf.Name));
        }
    }
    return NameString;
}

RotationSimple Rotation(Object* Obj)
{
    // calculate minimum rotation period to prevent disintegration
    float64 MinPeriod = cse::sqrt((4. * pow(CSE_PI, 2) * pow(Obj->Radius(), 3)) / (GravConstant * Obj->Mass));
    return
    {
        .RotationPeriod = random.uniform(MinPeriod, 36 * 3600),
        .Obliquity = random.uniform(0, 180),
        .EqAscendNode = random.uniform(0, 360)
    };
}

void OblateXZ(vec3* Dim)
{
    Dim->z = random.uniform(Dim->y, Dim->x);
    Dim->x = pow(Dim->x, 2) / Dim->z;
}

void CreateMoon(vector<Object>& Particles, const Object& Particle, const AsterBeltSettings& Conf, size_t Number)
{
    TerrestrialPlanetBase Model((Conf.MinRadius / RadEarth) / 5., (Particle.Dimensions.x / RadEarth) / 4., random.uniform(6.4, 7.6), random.uniform(2.8, 4.4));
    Object Moon = Model(random);
    float64 MinDist = RocheLimit(&Particle, &Moon, 1) + Moon.Radius() * 2.;
    Object Parent{ .Mass = Conf.CenterObjectMass };
    float64 MaxDist = HillSphere(&Parent, &Particle) / 2.5;
    if (MinDist >= MaxDist) { return; }

    float64 MinPeriod = GetPeriod(Particle.Mass, MinDist, 0);
    float64 MaxPeriod = GetPeriod(Particle.Mass, MaxDist, 0);

    Stanley_Dermott_Engine DistArr(random.uniform(MinPeriod, MaxPeriod), random.uniform(1.8, 3.2));
    uint64 i = 0;
    while (DistArr[i] < MaxPeriod && i < MaxMoonNumber)
    {
        Moon.Type = "Asteroid";
        Moon.Class = "Asteroid";
        Moon.ParentBody = Particle.Name[0];
        if (Conf.EnableRealNames) { Moon.Name = { vformat(RealDMoonString(i + 1, epoch::CSEDate::fromJulianDay(Conf.Epoch).year(), "DwarfMoon", vformat("({})", make_format_args(Number)), ""), make_format_args(Conf.Name)) }; }
        else { Moon.Name = { vformat(Moon.ParentBody + ".{}", make_format_args(i + 1)) }; }
        Moon.TidalLocked = true;
        Moon.Orbit =
        {
            .RefPlane        = "Equator",
            .Epoch           = Conf.Epoch,
            .PericenterDist  = GetSemiMajorAxis(Particle.Mass, DistArr[i]),
            .Eccentricity    = 0,
            .Inclination     = random.uniform(-0.01, 0.01),
            .AscendingNode   = random.uniform(0, 360),
            .ArgOfPericenter = random.uniform(0, 360),
            .MeanAnomaly     = random.uniform(0, 360)
        };

        Particles.push_back(Moon);
        Moon = Model(random);
        ++i;
    }
}

void CreateBinary(vector<Object>& Particles, Object& Particle, const AsterBeltSettings& Conf, size_t Number)
{
    TerrestrialPlanetBase Model((Conf.MinRadius / RadEarth) / 2., (Particle.Dimensions.x / RadEarth) / 1.1, random.uniform(6.4, 7.6), random.uniform(2.8, 4.4));
    Object Companion = Model(random);
    Companion.Type = "Asteroid";
    Companion.Class = "Asteroid";
    Object Parent{ .Mass = Conf.CenterObjectMass };

    Object Barycenter = *MakeBinary
    (
        &Particle, &Companion, &Parent,
        {
            .RefPlane = "Equator",
            .Epoch = Conf.Epoch,
            .Eccentricity = random.uniform(0, 0.1),
            .Inclination = random.uniform(-0.01, 0.01),
            .AscendingNode = random.uniform(0, 360),
            .ArgOfPericenter = random.uniform(0, 360),
            .MeanAnomaly = random.uniform(0, 360)
        }
    );

    if (Conf.EnableRealNames)
    {
        Barycenter.Name[0] = Particle.Name[0] + " AB";
        Companion.Name = { vformat(RealDMoonString(1, epoch::CSEDate::fromJulianDay(Conf.Epoch).year(), "DwarfMoon", vformat("({})", make_format_args(Number)), ""), make_format_args(Conf.Name)) };
    }
    else
    {
        Barycenter.Name = Particle.Name;
        Particle.Name[0] = Particle.Name[0] + " A";
        Companion.Name[0] = Particle.Name[0] + " B";
    }

    Particle.ParentBody = Barycenter.Name[0];
    Companion.ParentBody = Barycenter.Name[0];
    Particle.TidalLocked = true;
    Particle.Rotation = {};
    Companion.TidalLocked = true;
    Particles.push_back(Barycenter);
    Particles.push_back(Particle);
    Particles.push_back(Companion);
}

// ---------- Models ---------- //

void RingDistribution(vector<Object>& Particles, const AsterBeltSettings& Conf)
{
    for (size_t i = 0; i < Conf.NParticles; i++)
    {
        TerrestrialPlanetBase Model(Conf.MinRadius / RadEarth, Conf.MaxRadius / RadEarth, random.uniform(6.4, 7.6), random.uniform(2.8, 4.4));
        Object Particle = Model(random);
        Particle.Type = Conf.Type;
        Particle.Class = "Asteroid";
        Particle.ParentBody = Conf.CenterObjectName;
        Particle.Rotation = Rotation(&Particle);
        Oblate(&Particle);
        OblateXZ(&Particle.Dimensions);
        Particle.Orbit =
        {
            .RefPlane        = Conf.RefPlane,
            .Epoch           = Conf.Epoch,
            .PericenterDist  = random.uniform(Conf.InnerRadius, Conf.OuterRadius),
            .Eccentricity    = 0,
            .Inclination     = Conf.Inclination,
            .AscendingNode   = Conf.AscendingNode,
            .ArgOfPericenter = random.uniform(0, 360),
            .MeanAnomaly     = random.uniform(0, 360)
        };

        if (Conf.EnableRealNames) { Particle.Name = { NameStr(Conf, Particle, i + 1) }; }
        else 
        {
            if (Conf.Type == "Asteroid") { Particle.Name = { vformat(SEStyleAster, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "Comet") { Particle.Name = { vformat(SEStyleComet, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "DwarfMoon") { Particle.Name = { vformat(SEStyleDMoon, make_format_args(Conf.Name, i + 1)) }; }
        }

        if (Conf.Type == "Asteroid")
        {
            uint64 HaveMoon = random.probability({ 1. - Conf.ProbMoon, Conf.ProbMoon * (1. - Conf.ProbBinary), Conf.ProbMoon * Conf.ProbBinary });
            if (HaveMoon == 1)
            {
                Particles.push_back(Particle);
                CreateMoon(Particles, Particle, Conf, i + 1);
            }
            else if (HaveMoon == 2)
            {
                CreateBinary(Particles, Particle, Conf, i + 1);
            }
            else { Particles.push_back(Particle); }
        }
        else if (Conf.Type == "Comet")
        {
            Particle.AbsMagn = random.uniform(Conf.MaxAbsMagn, Conf.MinAbsMagn);
            Particles.push_back(Particle);
        }
        else { Particles.push_back(Particle); }
    }
}

void SphereDistribution(vector<Object>& Particles, const AsterBeltSettings& Conf)
{
    for (size_t i = 0; i < Conf.NParticles; i++)
    {
        TerrestrialPlanetBase Model(Conf.MinRadius / RadEarth, Conf.MaxRadius / RadEarth, random.uniform(6.4, 7.6), random.uniform(2.8, 4.4));
        Object Particle = Model(random);
        Particle.Type = Conf.Type;
        Particle.Class = "Asteroid";
        Particle.ParentBody = Conf.CenterObjectName;
        Particle.Rotation = Rotation(&Particle);
        Oblate(&Particle);
        OblateXZ(&Particle.Dimensions);

        float64 PericenterDist0 = random.uniform(Conf.InnerRadius, Conf.OuterRadius);
        float64 AphelionDistance = random.uniform(PericenterDist0, Conf.OuterRadius);
        float64 Eccentricity0 = (AphelionDistance - PericenterDist0) / (AphelionDistance + PericenterDist0);
        Particle.Orbit =
        {
            .RefPlane = Conf.RefPlane,
            .Epoch = Conf.Epoch,
            .PericenterDist = PericenterDist0,
            .Eccentricity = Eccentricity0,
            .Inclination = random.uniform(0, 180),
            .AscendingNode = random.uniform(0, 360),
            .ArgOfPericenter = random.uniform(0, 360),
            .MeanAnomaly = random.uniform(0, 360)
        };

        if (Conf.EnableRealNames) { Particle.Name = { NameStr(Conf, Particle, i + 1) }; }
        else
        {
            if (Conf.Type == "Asteroid") { Particle.Name = { vformat(SEStyleAster, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "Comet") { Particle.Name = { vformat(SEStyleComet, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "DwarfMoon") { Particle.Name = { vformat(SEStyleDMoon, make_format_args(Conf.Name, i + 1)) }; }
        }

        if (Conf.Type == "Asteroid")
        {
            uint64 HaveMoon = random.probability({ 1. - Conf.ProbMoon, Conf.ProbMoon * (1. - Conf.ProbBinary), Conf.ProbMoon * Conf.ProbBinary });
            if (HaveMoon == 1)
            {
                Particles.push_back(Particle);
                CreateMoon(Particles, Particle, Conf, i + 1);
            }
            else if (HaveMoon == 2)
            {
                CreateBinary(Particles, Particle, Conf, i + 1);
            }
            else { Particles.push_back(Particle); }
        }
        else if (Conf.Type == "Comet")
        {
            Particle.AbsMagn = random.uniform(Conf.MaxAbsMagn, Conf.MinAbsMagn);
            Particles.push_back(Particle);
        }
        else { Particles.push_back(Particle); }
    }
}

void TorusDistribution(vector<Object>& Particles, const AsterBeltSettings& Conf)
{
    for (size_t i = 0; i < Conf.NParticles; i++)
    {
        vec3 CenterPoint(0);
        // Generating Lat/Lon
        float64 IntersecRadius = (Conf.OuterRadius - Conf.InnerRadius) / 2.;
        float64 CenterRadius = Conf.InnerRadius + IntersecRadius;
        float64 InclVar = arcsin(IntersecRadius / CenterRadius);
        vec3 HPoint(random.uniform(0, 360), random.uniform(0, InclVar), 0);

        // Generating Distances
        vector<complex64> Distances;
        SolveQuadratic({ 1., -2. * CenterRadius * cse::cos(HPoint.y), pow(CenterRadius,2) - pow(IntersecRadius,2) }, Distances);
        float64 MinDist = min(Distances[0].real(), Distances[1].real());
        float64 MaxDist = max(Distances[0].real(), Distances[1].real());
        HPoint.z = random.uniform(MinDist, MaxDist);
        float64 AphelionDist = random.uniform(HPoint.z, MaxDist);
        float64 Eccentricity0 = (AphelionDist - HPoint.z) / (AphelionDist + HPoint.z);

        // Now, there are 2 points: Highest point("HPoint", Circular orbit by default), Center point(0, 0, 0)
        // Another point is needed to create orbital plane, Here use the ascending node.
        vec3 AscNode(HPoint.x - 90, 0, HPoint.z); // In fact, the result is not affect by distances.

        // Convert the Highest point and ascending node into XYZ coordinates
        // and create normal vector of this plane
        vec3 HPointXYZ = PolarToXYZ(HPoint);
        vec3 AscNodeXYZ = PolarToXYZ(AscNode);
        vec3 NormalVec = cross(HPointXYZ, AscNodeXYZ);

        // Rotate the normal vector base on X-axis by inclination
        vec2 NormalYZ(NormalVec.y, NormalVec.z);
        vec2 NormalYZPolar = XYToPolar(NormalYZ);
        NormalYZPolar.y += Conf.Inclination;
        vec2 NewNormYZ = PolarToXY(NormalYZPolar);
        vec3 NewNormVec(NormalVec.x, NewNormYZ); // Final plane normal vector

        // Then, find the new inclination and longitude of ascending node.
        vec3 RefNorm(0, 1, 0);
        float64 NewIncl = arccos(dot(NewNormVec, RefNorm) / (Length(NewNormVec) * Length(RefNorm)));
        float64 NewAscNode = arctan(NewNormVec.x / NewNormVec.z);
        //cout << NewIncl << ' ' << NewAscNode << '\n';

        TerrestrialPlanetBase Model(Conf.MinRadius / RadEarth, Conf.MaxRadius / RadEarth, random.uniform(6.4, 7.6), random.uniform(2.8, 4.4));
        Object Particle = Model(random);
        Particle.Type = Conf.Type;
        Particle.Class = "Asteroid";
        Particle.ParentBody = Conf.CenterObjectName;
        Particle.Rotation = Rotation(&Particle);
        Oblate(&Particle);
        OblateXZ(&Particle.Dimensions);
        Particle.Orbit =
        {
            .RefPlane = Conf.RefPlane,
            .Epoch = Conf.Epoch,
            .PericenterDist = HPoint.z,
            .Eccentricity = Eccentricity0,
            .Inclination = NewIncl,
            .AscendingNode = NewAscNode + Conf.AscendingNode,
            .ArgOfPericenter = random.uniform(0, 360),
            .MeanAnomaly = random.uniform(0, 360)
        };

        if (Conf.EnableRealNames) { Particle.Name = { NameStr(Conf, Particle, i + 1) }; }
        else
        {
            if (Conf.Type == "Asteroid") { Particle.Name = { vformat(SEStyleAster, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "Comet") { Particle.Name = { vformat(SEStyleComet, make_format_args(Conf.Name, i + 1)) }; }
            else if (Conf.Type == "DwarfMoon") { Particle.Name = { vformat(SEStyleDMoon, make_format_args(Conf.Name, i + 1)) }; }
        }

        if (Conf.Type == "Asteroid")
        {
            uint64 HaveMoon = random.probability({ 1. - Conf.ProbMoon, Conf.ProbMoon * (1. - Conf.ProbBinary), Conf.ProbMoon * Conf.ProbBinary });
            if (HaveMoon == 1)
            {
                Particles.push_back(Particle);
                CreateMoon(Particles, Particle, Conf, i + 1);
            }
            else if (HaveMoon == 2)
            {
                CreateBinary(Particles, Particle, Conf, i + 1);
            }
            else { Particles.push_back(Particle); }
        }
        else if (Conf.Type == "Comet")
        {
            Particle.AbsMagn = random.uniform(Conf.MaxAbsMagn, Conf.MinAbsMagn);
            Particles.push_back(Particle);
        }
        else { Particles.push_back(Particle); }
    }
}

// ---------- Generator ---------- //

void gen(const AsterBeltSettings& Config, OSCStream& fout)
{
    vector<Object> Particles;
    if (Config.Shape == "Ring") { RingDistribution(Particles, Config); }
    else if (Config.Shape == "Sphere") { SphereDistribution(Particles, Config); }
    else if (Config.Shape == "Torus") { TorusDistribution(Particles, Config); }
    fout << NoBooleans;
    fout.precision(15);

    for (size_t i = 0; i < Particles.size(); i++)
    {
        fout << Particles[i];
    }
    cout << vformat("{} Objects has written.\n", make_format_args(Particles.size()));
}

////////// MAIN //////////

int main()
{
    _WHILE_TRUE
    SetConsoleTitle(CSE_TITLE_STRING);
    CSEDateTime DateTime = CSEDateTime::currentDateTimeUTC();
    cout.precision(15);
    
    cout << "||==============================||\n";
    cout << "|| AsterBeltCreator C++ Edition ||\n";
    cout << "||==============================||\n";

    AsterBeltSettings Conf;

    GetInput(&Conf.Type, string("Asteroid"), LngStr02);
    if (Conf.Type == "1" || Conf.Type == "Asteroid") { Conf.Type = "Asteroid"; }
    else if (Conf.Type == "2" || Conf.Type == "Comet") { Conf.Type = "Comet"; }
    else if (Conf.Type == "3" || Conf.Type == "DwarfMoon") { Conf.Type = "DwarfMoon"; }
    else if (Conf.Type == "0" || Conf.Type == "x") { exit(0); }
    else { Conf.Type = "Asteroid"; }
    cout << Conf.Type << '\n';

    GetAllLine(&Conf.CenterObjectName, string("Sun"), LngStr05);
    cout << Conf.CenterObjectName << '\n';

    if (Conf.Type == "Asteroid")
    {
        GetInput(&Conf.CenterObjectMass, 1., LngStr07 + LngStr08 + LngStr09 + LngStr10);
        if (Conf.CenterObjectMass < 0) { Conf.CenterObjectMass = cse::abs(Conf.CenterObjectMass * MassEarth); }
        else { Conf.CenterObjectMass = Conf.CenterObjectMass * MassSol; } // Convert into Kg
        cout << Conf.CenterObjectMass << '\n';
    }

    GetInput(&Conf.EnableRealNames, false, LngStr32);
    cout << Conf.EnableRealNames << '\n';

    GetInput(&Conf.Name, Conf.CenterObjectName, vformat(LngStr04, make_format_args(Conf.CenterObjectName)));
    cout << Conf.Name << '\n';

    GetInput(&Conf.NParticles, 50ULL, LngStr03);
    cout << Conf.NParticles << '\n';

    GetInput(&Conf.MinRadius, 0.1, LngStr18);
    Conf.MinRadius *= 1000; // Convert to metres
    cout << Conf.MinRadius << '\n';

    GetInput(&Conf.MaxRadius, 50., LngStr19);
    Conf.MaxRadius *= 1000; // Convert to metres
    cout << Conf.MaxRadius << '\n';

    if (Conf.Type == "Comet")
    {
        GetInput(&Conf.MinAbsMagn, 5., LngStr33);
        cout << Conf.MinAbsMagn << '\n';

        GetInput(&Conf.MaxAbsMagn, -5., LngStr34);
        cout << Conf.MaxAbsMagn << '\n';
    }

    GetInput(&Conf.Shape, string("Torus"), LngStr31);
    if (Conf.Shape == "1" || Conf.Shape == "Ring") { Conf.Shape = "Ring"; }
    else if (Conf.Shape == "2" || Conf.Shape == "Sphere") { Conf.Shape = "Sphere"; }
    else if (Conf.Shape == "3" || Conf.Shape == "Torus") { Conf.Shape = "Torus"; }
    else { Conf.Shape = "Torus"; }
    cout << Conf.Shape << '\n';

    GetInput(&Conf.InnerRadius, 5., LngStr11 + LngStr12 + LngStr14);
    if (Conf.InnerRadius < 0) { Conf.InnerRadius = cse::abs(Conf.InnerRadius * 1000); }
    else { Conf.InnerRadius = Conf.InnerRadius * AU; } // Convert into Metres
    cout << Conf.InnerRadius << '\n';

    GetInput(&Conf.OuterRadius, 6., LngStr15 + LngStr16 + LngStr17);
    if (Conf.OuterRadius < 0) { Conf.OuterRadius = cse::abs(Conf.OuterRadius * 1000); }
    else { Conf.OuterRadius = Conf.OuterRadius * AU; } // Convert into Metres
    cout << Conf.OuterRadius << '\n';

    if (Conf.Shape != "Sphere")
    {
        GetInput(&Conf.Inclination, 0., LngStr26);
        cout << Conf.Inclination << '\n';

        GetInput(&Conf.AscendingNode, 0., LngStr27);
        cout << Conf.AscendingNode << '\n';
    }

    GetInput(&Conf.RefPlane, string("Equator"), LngStr23);
    if (Conf.RefPlane == "1" || Conf.RefPlane == "Ecliptic") { Conf.RefPlane = "Ecliptic"; }
    else if (Conf.RefPlane == "2" || Conf.RefPlane == "Equator") { Conf.RefPlane = "Equator"; }
    else if (Conf.RefPlane == "3" || Conf.RefPlane == "Extrasolar") { Conf.RefPlane = "Extrasolar"; }
    else { Conf.RefPlane = "Equator"; }
    cout << Conf.RefPlane << '\n';

    GetInput(&Conf.Epoch, DateTime.date().toJulianDay() + TimeToJDFract(DateTime.time()), LngStr24);
    cout << Conf.Epoch << '\n';

    GetInput(&Conf.OutputFileName, Conf.CenterObjectName + '_' + Conf.Type, vformat(LngStr25, make_format_args(Conf.CenterObjectName + '_' + Conf.Type)));
    ofstream fout(Conf.OutputFileName + ".sc");
    OSCStream File(fout);
    gen(Conf, File);
    File.write();
    fout.close();

    _END_WHILE_TRUE
}
