#include <iostream>
#include <fstream>
#include <format>
#include "CSE/Lumine.h"

using namespace std;
using namespace cse;

int main()
{
    ofstream Report("TestReport_SpecType.txt");
    // Single type parse
    cout << SPECSTR("G2V").str() << '\n';
    Report << SPECSTR("G2V").str() << '\n';
    cout << SPECSTR("M5.2III").str() << '\n';
    Report << SPECSTR("M5.2III").str() << '\n';
    cout << SPECSTR("DB3.1").str() << '\n';
    Report << SPECSTR("DB3.1").str() << '\n';
    cout << SPECSTR("sdB5").str() << '\n';
    Report << SPECSTR("sdB5").str() << '\n';
    // Single types with spaces
    cout << SPECSTR("G2 V").str() << '\n';
    Report << SPECSTR("G2 V").str() << '\n';
    cout << SPECSTR("M5.2 III").str() << '\n';
    Report << SPECSTR("M5.2 III").str() << '\n';
    cout << SPECSTR("DB 3.1").str() << '\n';
    Report << SPECSTR("DB 3.1").str() << '\n';
    // Single types lack of indexes
    cout << SPECSTR("G2").str() << '\n';
    Report << SPECSTR("G2").str() << '\n';
    cout << SPECSTR("M III").str() << '\n';
    Report << SPECSTR("M III").str() << '\n';
    cout << SPECSTR("K").str() << '\n';
    Report << SPECSTR("K").str() << '\n';
    // Multiple types
    cout << SPECSTR("A3-4III-IV").str() << '\n';
    Report << SPECSTR("A3-4III-IV").str() << '\n';
    cout << SPECSTR("Of/WNL").str() << '\n';
    Report << SPECSTR("Of/WNL").str() << '\n';
    cout << SPECSTR("Ofpe/WN9").str() << '\n';
    Report << SPECSTR("Ofpe/WN9").str() << '\n';
    cout << SPECSTR("O2-3.5If*/WN5-7").str() << '\n';
    Report << SPECSTR("Ofpe/WN9").str() << '\n';
    cout << SPECSTR("DQZ").str() << '\n';
    Report << SPECSTR("DQZ").str() << '\n';
    // Pecular Types
    cout << SPECSTR("B1.5Vnne").str() << '\n';
    Report << SPECSTR("B1.5Vnne").str() << '\n';
    cout << SPECSTR("ApSi").str() << '\n';
    Report << SPECSTR("ApSi").str() << '\n';
    cout << SPECSTR("A3VpSrCrEu").str() << '\n';
    Report << SPECSTR("A3VpSrCrEu").str() << '\n';
    cout << SPECSTR("sdOHe").str() << '\n';
    Report << SPECSTR("sdOHe").str() << '\n';
    // S-Type stars
    cout << SPECSTR("S2,5").str() << '\n';
    Report << SPECSTR("S2,5").str() << '\n';
}
