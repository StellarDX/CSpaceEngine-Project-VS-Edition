#pragma once

// This file include some constants for astronomical calculating
// all values are in SI base unit unless otherwise explicitly stated.

#ifndef _AS_CONSTANTS
#define _AS_CONSTANTS

// Define _USE_CSE_DEFINES before including "CSEConstants.h" to expose these definitions for constants.

#if defined(_USE_CSE_DEFINES)

#if !defined(_CSE_MATH_MODULE)
#include <cmath> // Use STD math when CSE Math module is missing.
#endif

// List1: Math Defines
// -------------------------------------------------------------------------
//      Name          Value                                  Note
#define CSE_E         2.7182818284590452353602874713527l  // e
#define CSE_LOG2E     1.4426950408889634073599246810019l  // log(2, e)
#define CSE_LOG10E    0.43429448190325182765112891891661l // log(e)
#define CSE_LN2       0.69314718055994530941723212145818l // ln(2)
#define CSE_LN10      2.3025850929940456840179914546844l  // ln(10)
#define CSE_PI        3.1415926535897932384626433832795l  // Pi
#define CSE_PI_D2     1.5707963267948966192313216916398l  // Pi / 2
#define CSE_PI_D4     0.78539816339744830961566084581988l // Pi / 4
#define CSE_1D_PI     0.31830988618379067153776752674503l // 1 / Pi
#define CSE_2D_PI     0.63661977236758134307553505349006l // 2 / Pi
#define CSE_2D_SQRTPI 1.1283791670955125738961589031215l  // 2 / sqrt(Pi)
#define CSE_SQRT2     1.4142135623730950488016887242097l  // sqrt(2)
#define CSE_SQRT1_D2  0.70710678118654752440084436210485l // sqrt(1 / 2)


// List2: Common physic constants
// -------------------------------------------------------------------------
//      Name           Value/Equation       Note
#define GravConstant   6.67430E-11       // (±0.00015)2018 CODATA Value, reference from NIST
#define SpeedOfLight   299792458         // Exact value defined by 1983 17th CGPM
#define PPM            0.0001            // by definition, Parts-per million to percent
#define BoltzmannConst 1.380649E-23      // SI by definition, J/K = m2⋅kg/(s2⋅K) in SI base units.
#define PlanckConst    6.62607015E-34    // 2018 CODATA Value
#define StBConstant    Equation21        // by definition
#define GasConstant    8.31446261815324  // 2018 CODATA Value
#define AvogadroConst  6.02214076E+23    // 2018 CODATA Value
#define Dalton         1.66053906660E-27 // (±0.0000000000050)
// -------------------------------------------------------------------------
const long double Equation21 = (2.0 * pow(CSE_PI, 5) * pow(BoltzmannConst, 4)) / (15.0 * pow(PlanckConst, 3) * pow(SpeedOfLight, 2));


// List3: Units of mass
// Reference: https://ssd.jpl.nasa.gov/?planet_phys_par
// -------------------------------------------------------------------------
//      Name          Value          Note
#define MassEarth     5.97217E+24  // (±0.00028)Estimated value from planetary ephemeris DE421
#define MassJupiter   1.898125E+27 // (±0.000088)value from Jovian Satellite ephemeris JUP310
#define MassSol       1.98847E+30  // (±0.00007)given by solving Kepler's third law


// List4: Units of length
// -------------------------------------------------------------------------
//      Name          Value/Equation      Note
#define RadEarth      6378137          // equatorial radius provided by IUGG
#define RadJupiter    71492000         // IAU 2015 definition
#define RadSol        695700000        // IAU 2015 definition
#define AU            149597870700     // IAU 2012 definition
#define LightYear     9460730472580800 // IAU 1976 System of Astronomical Constants
#define Parsec        Equation41       // exact by the 2015 definition
// -------------------------------------------------------------------------
const long double Equation41 = (180.0 * 60.0 * 60.0 * AU) / CSE_PI;


// List5: Units of Time
// -------------------------------------------------------------------------
//      Name          Value                Note
#define SynodicDay    86400             // mean synodic day(Solar day) in seconds(Sunrise to Sunrise), is approx. 24 hours
#define SiderealDay   86164.09053083288 // Earth's rotation period relative to the precessing mean vernal equinox
#define TropicalYear  31556925.18747072 // mean tropical year on January 1, 2000, equals 365.2421896698 days
#define SiderealYear  31558149.7635456  // J2000.0 epoch, equals 365.256363004 days
#define J2000         2451545           // Julian day of 2000.01.01 12:00:00.00


// List6: Units of Luminosity
// -------------------------------------------------------------------------
//      Name          Value        Note
#define SolarLum      3.828E+26 // nominal solar luminosity defined by IAU


// List7: Molar Masses
// Values computed by PubChem 2.1 (PubChem release 2021.05.07): https://pubchem.ncbi.nlm.nih.gov/
// -------------------------------------------------------------------------
//      Name          Value        Exact Value
#define CSEM_H2       2.016     // 2.0156500638
#define CSEM_He       4.00260   // 4.002603254
#define CSEM_Ne       20.180    // 19.99244018
#define CSEM_Ar       39.9      // 39.96238312
#define CSEM_Kr       83.80     // 83.91149773
#define CSEM_Xe       131.29    // 131.90415508
#define CSEM_O2       31.999    // 31.989829239
#define CSEM_N2       28.014    // 28.006148008
#define CSEM_CO       28.010    // 27.994914619
#define CSEM_CO2      44.009    // 43.989829239
#define CSEM_SO       48.07     // 47.96698579
#define CSEM_SO2      64.07     // 63.96190041
#define CSEM_Cl2      70.90     // 69.9377054 (35Cl only?)
#define CSEM_H2S      34.08     // 33.98772124
#define CSEM_H2O      18.015    // 18.010564683
#define CSEM_NH3      17.031    // 17.026549100
#define CSEM_CH4      16.043    // 16.0313001276
#define CSEM_C2H2     26.04     // 26.0156500638
#define CSEM_C2H4     28.05     // 28.0313001276
#define CSEM_C2H6     30.07     // 30.0469501914
#define CSEM_C3H8     44.10     // 44.062600255
#define CSEM_C8H18    114.23    // 114.140850574


// List8: Cosmology
// Can't promise that these values are correct.
// Because I don't know why I want to add these "meaningless" constants.
// Reference:
// https://en.wikipedia.org/wiki/Cosmological_constant
// https://en.wikipedia.org/wiki/Planck_units
// https://en.wikipedia.org/wiki/Hubble%27s_law
// -------------------------------------------------------------------------
//      Name              Value              Note
#define ReducedPlkConst   Equation81       // reduced Planck constant(Format: eV/s)
#define PlanckLength      Equation82       // shortest measurable distance, approximately the size of a black hole where quantum and gravitational effects are at the same scale
#define PlanckMass        Equation83       // The dividing point between macro scale and micro scale
#define PlanckTime        Equation84       // the time it takes for a photon to travel a distance equal to the Planck length, and is the shortest possible time interval that can be measured.
#define PlanckTemp        Equation85       // At this temperature, the wavelength of light emitted by thermal radiation reaches the Planck length
#define HubbleParam       73.4             // (+0.99, −1.22 | Format: (Km/s)/Mpc)Value by Pantheon+, 2022-02-08, SN Ia distance ladder (+SH0ES) (preprint)
#define HubbleParamSI     Equation86       // Hubble Parameter in SI (Format: 1/s)
#define CosmoDensityRatio 0.6889           // (±0.0056)ratio between the energy density due to the cosmological constant and the critical density of the universe, according to results published by the Planck Collaboration in 2018
#define CosmoConstant     Equation87       // constant coefficient of a term Albert Einstein temporarily added to his field equations of general relativity
#define StarLowMassLimit  1.4894586875E+29 // Dividing point between Brown dwarf and star, Result from Universe sandbox experiment(78.47MJ)
#define ChandrasekharLim  2.765E+30        // maximum mass of a stable white dwarf star
#define StarWDUpLimExp    1.590776E+31     // Result from Universe sandbox experiment(8MSun)
#define TOVLimit          4.7126739E+30    // an upper bound to the mass of cold, nonrotating neutron stars
#define StarBHLowLimExp   7.2910566667E+31 // Result from Universe sandbox experiment(36.6667MSun)
#define UpMassGapBH       8.948115E+31     // Stars more massive than this value will remain nothing after supernova explotion.
#define RadObsUniverse    8.8E+26          // Radius of Observable universe(in meters)
#define HubbleLength      Equation88       // The proper radius of a Hubble sphere
// -------------------------------------------------------------------------
const long double Equation81 = PlanckConst / (2.0 * CSE_PI);
const long double Equation82 = sqrt((ReducedPlkConst * GravConstant) / pow(SpeedOfLight, 3));
const long double Equation83 = sqrt((ReducedPlkConst * SpeedOfLight) / GravConstant);
const long double Equation84 = sqrt((ReducedPlkConst * GravConstant) / pow(SpeedOfLight, 5));
const long double Equation85 = sqrt((ReducedPlkConst * pow(SpeedOfLight, 5)) / (GravConstant * pow(BoltzmannConst, 2)));
const long double Equation86 = (HubbleParam * 1000.0) / (1000000.0 * Parsec);
const long double Equation87 = 3.0 * pow(HubbleParamSI / SpeedOfLight, 2) * CosmoDensityRatio;
const long double Equation88 = SpeedOfLight / HubbleParamSI;

#endif

#endif
