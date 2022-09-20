// Star Luminosity and Magnitude Calculator Based on CSE
// This program's early versions are in C language
// And refactored in C++

// ----------Defines---------- //

#define _USE_CSE_DEFINES

// ----------Includes---------- //

#include <iostream>
#include <cstdlib>
#include <format>
#include "../headers/lumine/Lumine.h"
#include <windows.h>

// ----------Values---------- //

using cse::float64;

float64 AppMagn;
float64 AppMagn1;
float64 AppMagn2;
float64 Parallax;
float64 Dist;
float64 DistModulus;
float64 AbsMagn;

float64 Mass;
float64 Luminosity;
float64 LumBol;
float64 Teff;

float64 Radius;

using std::cout;
using std::cin;

// ----------Functions---------- //

/*AppMagn*/

void ToAppMagn1(float64 AbsMagn, float64 Dist)
{
    cout << "Star Absolute Magnitude: ";
    cin >> AbsMagn;
    cout << AbsMagn << '\n';

    cout << "Distance in Parsec: \nIf you have Ly, put a '-' sign for the number\n";
    cin >> Dist;

    if (Dist < 0)
    {
        Dist = 0 - Dist / (Parsec / LightYear);
    }

    cout << Dist << '\n';

    cout << std::format("AppMagn: {}\n", cse::ToAppMagn1(AbsMagn, Dist));
}

void ToAppMagn2(float64 AppMagn1, float64 AppMagn2)
{
    cout << "Obj1 Magnitude: ";
    cin >> AppMagn1;
    cout << AppMagn1 << '\n';

    cout << "Obj2 Magnitude: ";
    cin >> AppMagn2;
    cout << AppMagn2 << '\n';

    if (AppMagn1 < AppMagn2)
    {
        std::swap(AppMagn1, AppMagn2);
    }

    float64 X;
    float64 vb;
    X = AppMagn1 - AppMagn2;
    vb = pow(10, 0.4 * X);
    cout << std::format("The brighter object appears about {} times brighter than the fainter object.\n", vb);
}

/*AbsMagn*/

void ToAbsMagn1(float64 AppMagn, float64 Dist)
{
    cout << "Star Apparent Magnitude: ";
    cin >> AppMagn;
    cout << AppMagn << '\n';

    cout << "Distance in Parsec: \nIf you have Ly, put a '-' sign for the number\n";
    cin >> Dist;

    if (Dist < 0)
    {
        Dist = 0 - Dist / (Parsec / LightYear);
    }

    cout << Dist << '\n';

    cout << std::format("AbsMagn: {}\n", cse::ToAbsMagn1(AppMagn, Dist));
}

void ToAbsMagn2(float64 Parallax, float64 AppMagn)
{
    cout << "Parallax: ";
    cin >> Parallax;
    cout << Parallax << '\n';

    cout << "Star Apparent Magnitude: ";
    cin >> AppMagn;
    cout << AppMagn << '\n';

    cout << std::format("AbsMagn: {}\n", cse::ToAbsMagn2(Parallax, AppMagn));
}

void ToAbsMagn3(float64 AppMagn, float64 DistModulus)
{
    cout << "Apparent Magnitude: ";
    cin >> AppMagn;
    cout << AppMagn << '\n';

    cout << "Distance Modulus: ";
    cin >> DistModulus;
    cout << DistModulus << '\n';

    cout << std::format("AbsMagn: {}\n", AppMagn - DistModulus);
}

void ToAbsMagn4(float64 LumBol)
{
    cout << "Bolometric Luminosity in Watts: \nIf you have LSun, put a '-' sign for the number\n";
    cin >> LumBol;

    if (LumBol < 0)
    {
        LumBol = 0 - (LumBol * SolarLum);
    }

    cout << LumBol << '\n';

    cout << std::format("AbsMagnBol: {}\n", cse::ToAbsMagn4(LumBol));
}

/*Luminosity*/

void ToLuminosity1(float64 Radius, float64 Teff)
{
    cout << "Radius in Metres: \n";
    cout << "If you have SolarRadius, put a '-' sign for the number\n";
    cin >> Radius;

    if (Radius < 0)
    {
        Radius = abs(Radius * RadSol);
    }

    cout << Radius << '\n';
    float64 SurfArea = 4. * CSE_PI * pow(Radius, 2);

    cout << "Temperature in Kelvins: ";
    cin >> Teff;
    cout << Teff << '\n';

    Luminosity = cse::ToLuminosity1(Radius, Teff);
    float64 FluxDensity;
    FluxDensity = Luminosity / SurfArea;

    cout << "Output format: \n 0 = Watts\n 1 = LSun\n";
    cout << "Default: 0\n";
    int OutFormat;
    cin >> OutFormat;

    float64 LSun;
    cout << std::format("Stefan-Boltzmann constant: {} W/(m^2*K^4)\n", StBConstant);
    switch (OutFormat)
    {
    case 0:
        cout << std::format("Luminosity: {} W\n", Luminosity);
        break;

    case 1:
        LSun = Luminosity / SolarLum;
        cout << std::format("Luminosity: {} LSun\n", LSun);
        break;

    default:
        cout << std::format("Luminosity: {} W\n", Luminosity);
        break;
    }
    cout << std::format("Flux Density: {} W/m^2\n", FluxDensity);
}

void ToLuminosity2(float64 Mass)
{
    cout << "Star's Mass in MSun: ";
    cin >> Mass;
    cout << Mass << '\n';

    if (Mass > 0.2 && Mass < 0.85)
    {
        cout << "This star seems to be a K-type star, do you want to open precise mode?\n 1 = true\n 0 = false\n";
        cout << "Default: 1\n";
        int KStar;
        cin >> KStar;

        float64 A;
        switch (KStar)
        {
        case 1:
            A = -141.7 * pow(Mass, 4) + 232.4 * pow(Mass, 3) - 129.1 * pow(Mass, 2) + 33.29 * Mass + 0.215;
            Luminosity = pow(Mass, A);
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
            break;

        case 0:
            if (Mass < 0.43)
            {
                Luminosity = 0.23 * pow(Mass, 2.3);
                cout << std::format("Luminosity: {} LSun\n", Luminosity);
            }

            else if (Mass >= 0.43)
            {
                Luminosity = pow(Mass, 4);
                cout << std::format("Luminosity: {} LSun\n", Luminosity);
            }

            break;

        default:
            A = -141.7 * pow(Mass, 4) + 232.4 * pow(Mass, 3) - 129.1 * pow(Mass, 2) + 33.29 * Mass + 0.215;
            Luminosity = pow(Mass, A);
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
            break;
        }
    }

    else
    {
        if (Mass < 0.43)
        {
            Luminosity = 0.23 * pow(Mass, 2.3);
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
        }

        else if (Mass >= 0.43 && Mass < 2)
        {
            Luminosity = pow(Mass, 4);
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
        }

        else if (Mass >= 2 && Mass < 55)
        {
            Luminosity = 1.4 * pow(Mass, 3.5);
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
        }

        else if (Mass >= 55)
        {
            Luminosity = 32000 * Mass;
            cout << std::format("Luminosity: {} LSun\n", Luminosity);
        }
    }
}

void ToLuminosity3(float64 AbsMagnBol)
{
    cout << "Absolute Bolometric Magnitude: ";
    cin >> AbsMagnBol;
    cout << AbsMagnBol << '\n';

    LumBol = cse::ToLuminosity3(AbsMagnBol);

    cout << "Output format: \n 0 = Watts\n 1 = LSun\n";
    cout << "Default: 0\n";
    int OutFormat;
    cin >> OutFormat;

    float64 LSun;
    switch (OutFormat)
    {
    case 0:
        cout << std::format("LumBol: {} W\n", LumBol);
        break;

    case 1:
        LSun = LumBol / SolarLum;
        cout << std::format("LumBol: {} LSun\n", LSun);
        break;

    default:
        cout << std::format("LumBol: {} W\n", LumBol);
        break;
    }
}

void SelectAppMagnMode()
{
    cout << "Apparent Magnitude can be measured directly, so these calculation results are only for reference.\n";
    cout << "Convert from: \n 1 = AbsMagn and Dist\n 2 = Comparing the brightness of two objects\n 0 = Back\n";
    cout << "Default: 1\n";
    int ToAppMagn;
    cin >> ToAppMagn;

    switch (ToAppMagn)
    {
    case 1:
        ToAppMagn1(AbsMagn, Dist);
        break;

    case 2:
        ToAppMagn2(AppMagn1, AppMagn2);
        break;

    case 0:
        break;

    default:
        ToAppMagn1(AbsMagn, Dist);
        break;
    }
}

void SelectAbsMagnMode()
{
    cout << "Convert from: \n 1 = AppMagn and Dist\n 2 = Parallax and AppMagn\n 3 = AppMagn and DistModulus\n 4 = LumBol\n 0 = Back\n";
    cout << "Default: 4\n";
    int ToAbsMagn;
    cin >> ToAbsMagn;

    switch (ToAbsMagn)
    {
    case 1:
        ToAbsMagn1(AppMagn, Dist);
        break;

    case 2:
        ToAbsMagn2(Parallax, AppMagn);
        break;

    case 3:
        ToAbsMagn3(AppMagn, DistModulus);
        break;

    case 4:
        ToAbsMagn4(LumBol);
        break;

    case 0:
        break;

    default:
        ToAbsMagn4(LumBol);
        break;
    }
}

void SelectLuminosityMode()
{
    cout << "Convert from: \n 1 = SurfArea and Teff(Stefan-Boltzmann equation)\n 2 = Mass(Only for Main Sequence)\n 3 = AbsMagnBol\n 0 = Back\n";
    cout << "Default: 1\n";
    int ToLuminosity;
    cin >> ToLuminosity;

    switch (ToLuminosity)
    {
    case 1:
        ToLuminosity1(Radius, Teff);
        break;

    case 2:
        ToLuminosity2(Mass);
        break;

    case 3:
        ToLuminosity3(AbsMagn);
        break;

    case 0:
        break;

    default:
        ToLuminosity1(Radius, Teff);
        break;
    }
}

// ----------Main---------- //

int main(void)
{
    SetConsoleTitle(CSE_TITLE_STRING);

    while(true)
    {
        cout << "\nStar Luminosity and Magnitude Calculator\n(Based on CSpaceEngine)\n----------------------------------------------\n";
        cout << "Mode\n 1 = To AppMagn\n 2 = To AbsMagn\n 3 = To Luminosity\n 0 = Exit\n";
        cout << "Default: 2\n";
        int Preset;
        cin >> Preset;

        switch (Preset)
        {
        case 1:
            SelectAppMagnMode();
            break;

        case 2:
            SelectAbsMagnMode();
            break;

        case 3:
            SelectLuminosityMode();
            break;

        case 0:
            printf("Exiting...");
            exit(0);

        default:
            SelectAbsMagnMode();
            break;
        }
    }
}

