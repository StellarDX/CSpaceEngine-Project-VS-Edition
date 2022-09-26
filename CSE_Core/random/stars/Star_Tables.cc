#include "../../headers/random/stars.h"

_CSE_BEGIN

//===========================================================================//
//                                                                           //
//                       Main Sequence star tables                           //
//                                                                           //
//===========================================================================//
// Reference: Mamajek, Eric. "A Modern Mean Dwarf Stellar Color and Effective Temperature Sequence". 
// University of Rochester, Department of Physics and Astronomy.
// http://www.pas.rochester.edu/~emamajek/EEM_dwarf_UBVIJHK_colors_Teff.txt
// Define "USE_REAL_STAR_DATA" to use real star data instead of original data(if have).
// Real data reference:
// https://en.wikipedia.org/wiki/Stellar_classification

const float64 inf = NO_DATA_FLOAT_INF;

_RAND_BEGIN

static const STPARS
_O_type_main_sequence_star_Table[13]
(
// Data of early O-type stars are uncertain.
// No data for stars earlier than O2. Maybe thay are exist but un-discovered.
// O-type standards between O5 and O9 are listed by Morgan-Keenan-Kellerman, 1943
// And subtypes earlier than O5 is defined by Morgan, Abt, & Tapscott in 1970
//   SpT  Teff   BCv    Mbol    R_RSun MSun
	{2.0, 54000, -4.74, -10.44, 13.90, 97.6}, // using data of star "BI 253" for O2-class stars.
	{3.0, 44900, -4.01,  -9.81, 13.43, 59.0},
	{4.0, 42900, -3.89,  -9.39, 12.13, 48.0},
	{5.0, 41400, -3.76,  -9.11, 11.45, 43.0},
	{5.5, 40500, -3.67,  -8.87, 10.71, 38.0},
	{6.0, 39500, -3.57,  -8.67, 10.27, 35.0},
	{6.5, 38300, -3.49,  -8.44,  9.82, 31.0},
	#if defined(USE_REAL_STAR_DATA)
	{7.0, 38500, -3.38,  -8.59,  9.90, 29.1}, // O7 standard star: S Mon Aa
	#else
	{7.0, 37100, -3.41,  -8.21,  9.42, 28.0},
	#endif
	{7.5, 36100, -3.33,  -7.98,  8.95, 26.0},
	{8.0, 35100, -3.24,  -7.74,  8.47, 23.6},
	{8.5, 34300, -3.18,  -7.53,  8.06, 21.9},
	#if defined(USE_REAL_STAR_DATA)
	{9.0, 36000, -3.38,  -7.78,  8.27, 26.9}, // O9 standard star: 10 Lac
	#else
	{9.0, 33300, -3.11,  -7.31,  7.72, 20.2},
	#endif
	{9.5, 31900, -3.01,  -7.06,  7.50, 18.7}
);

static const STPARS
_B_type_main_sequence_star_Table[14]
(
//   SpT  Teff   BCv    Mbol   R_RSun MSun
	#if defined(USE_REAL_STAR_DATA)
	{0.0, 33400, -3.07, -6.87, 7.50, 16.60}, // B0 standard star: UPS Ori
	#else
	{0.0, 31400, -2.99, -6.89, 7.16, 17.70},
	#endif
	{0.5, 29000, -2.83, -6.33, 6.48, 14.80},
	{1.0, 26000, -2.58, -5.58, 5.71, 11.80},
	{1.5, 24500, -2.44, -5.04, 5.02,  9.90},
	{2.0, 20600, -2.03, -3.83, 4.06,  7.30},
	{2.5, 18500, -1.77, -3.27, 3.89,  6.10},
	#if defined(USE_REAL_STAR_DATA)
	{3.0, 15540, -1.52, -2.19, 3.40,  6.10}, // B3 standard star: ETA UMa
	#else
	{3.0, 17000, -1.54, -2.74, 3.61,  5.40},
	#endif
	{4.0, 16400, -1.49, -2.49, 3.46,  5.10},
	{5.0, 15700, -1.34, -2.19, 3.36,  4.70},
	{6.0, 14500, -1.13, -1.68, 3.27,  4.30},
	{7.0, 14000, -1.05, -1.45, 2.94,  3.92},
	{8.0, 12300, -0.73, -0.73, 2.86,  3.38},
	{9.0, 10700, -0.42,  0.08, 2.49,  2.75},
	{9.5, 10400, -0.36,  0.24, 2.45,  2.68}
);

static const STPARS
_A_type_main_sequence_star_Table[10]
(
// Original data has errors in stellar masses
// Correction by StellarDX.
// Reference: https://en.wikipedia.org/wiki/A-type_main-sequence_star
//   SpT  Teff  BCv     Mbol  R_RSun MSun
	#if defined(USE_REAL_STAR_DATA)
	{0.0, 9602, +0.149, 0.731, 2.818, 2.135}, // A0 standard star: Vega
	{1.0, 9940, -0.192, 1.228, 1.711, 2.063}, // A1 standard star: Sirius A
	#else
	{0.0, 9700, -0.210, 0.780, 2.193, 2.180},
	{1.0, 9300, -0.140, 1.020, 2.136, 2.050},
	#endif
	{2.0, 8800, -0.070, 1.280, 2.117, 1.980},
	#if defined(USE_REAL_STAR_DATA)
	{3.0, 8590, -0.032, 1.688, 1.842, 1.920}, // A3 standard star: Fomalhaut
	#else
	{3.0, 8600, -0.040, 1.660, 1.861, 1.930},
	#endif
	{4.0, 8250, -0.020, 1.920, 1.794, 1.880},
	{5.0, 8100,  0.000, 2.010, 1.785, 1.860},
	{6.0, 7910,  0.005, 2.130, 1.775, 1.830},
	{7.0, 7760,  0.010, 2.240, 1.750, 1.810},
	{8.0, 7590,  0.020, 2.340, 1.747, 1.770},
	{9.0, 7400,  0.020, 2.450, 1.747, 1.750}
);

static const STPARS
_F_type_main_sequence_star_Table[11]
(
//   SpT  Teff  BCv     Mbol  R_RSun MSun
	{0.0, 7220,  0.010, 2.58, 1.728, 1.61},
	{1.0, 7020,  0.005, 2.77, 1.679, 1.50},
	#if defined(USE_REAL_STAR_DATA)
	{2.0, 6908, -0.000, 2.84, 1.620, 1.34}, // F2 standard star: 78 UMa A
	#else
	{2.0, 6820, -0.005, 2.97, 1.622, 1.46},
	#endif
	{3.0, 6750, -0.010, 3.07, 1.578, 1.44},
	{4.0, 6670, -0.015, 3.19, 1.533, 1.38},
	{5.0, 6550, -0.020, 3.35, 1.473, 1.33},
	{6.0, 6350, -0.030, 3.66, 1.359, 1.25},
	{7.0, 6280, -0.035, 3.77, 1.324, 1.21},
	{8.0, 6180, -0.040, 4.01, 1.221, 1.18},
	{9.0, 6050, -0.050, 4.20, 1.167, 1.13},
	{9.5, 5990, -0.060, 4.29, 1.142, 1.08}
);

static const STPARS
_G_type_main_sequence_star_Table[10]
(
//   SpT  Teff  BCv     Mbol  R_RSun MSun
	#if defined(USE_REAL_STAR_DATA)
	{0.0, 6043, -0.053, 4.59, 1.123, 0.970}, // G0 standard star: BET CVn
	#else
	{0.0, 5930, -0.065, 4.42, 1.100, 1.060},
	#endif
	{1.0, 5860, -0.073, 4.55, 1.060, 1.030},
	#if defined(USE_REAL_STAR_DATA)
	{2.0, 5772, -0.090, 4.74, 1.000, 1.000}, // G2 standard star: The Sun
	#else
	{2.0, 5770, -0.085, 4.72, 1.012, 1.000},
	#endif
	{3.0, 5720, -0.095, 4.78, 1.002, 0.990},
	{4.0, 5680, -0.100, 4.83, 0.991, 0.985},
	#if defined(USE_REAL_STAR_DATA)
	{5.0, 5708, -0.244, 4.92, 0.950, 1.037}, // G5 standard star: KAP1 Cet
	#else
	{5.0, 5660, -0.105, 4.88, 0.977, 0.980},
	#endif
	{6.0, 5600, -0.115, 4.99, 0.949, 0.970},
	{7.0, 5550, -0.125, 5.08, 0.927, 0.950},
	#if defined(USE_REAL_STAR_DATA)
	{8.0, 5488, -0.252, 5.28, 0.860, 0.930}, // G8 standard star: 61 UMa
	#else
	{8.0, 5480, -0.140, 5.16, 0.914, 0.940},
	#endif
	{9.0, 5380, -0.160, 5.39, 0.853, 0.900}
);

static const STPARS
_K_type_main_sequence_star_Table[10]
(
//   SpT  Teff  BCv     Mbol  R_RSun MSun
	#if defined(USE_REAL_STAR_DATA)
	{0.0, 5255, -0.182, 5.71, 0.776, 0.85}, // K0 standard star: SIG Dra
	#else
	{0.0, 5270, -0.195, 5.59, 0.813, 0.88},
	#endif
	{1.0, 5170, -0.230, 5.72, 0.797, 0.86},
	#if defined(USE_REAL_STAR_DATA)
	{2.0, 5084, -0.279, 5.91, 0.735, 0.82}, // K2 standard star: EPS Eri
	#else
	{2.0, 5100, -0.260, 5.81, 0.783, 0.82},
	#endif
	{3.0, 4830, -0.375, 6.13, 0.755, 0.78},
	{4.0, 4600, -0.520, 6.46, 0.713, 0.73},
	#if defined(USE_REAL_STAR_DATA)
	{5.0, 4526, -0.728, 6.78, 0.665, 0.70}, // K5 standard star: 61 Cyg A
	#else
	{5.0, 4440, -0.630, 6.65, 0.701, 0.70},
	#endif
	{6.0, 4300, -0.750, 6.89, 0.669, 0.69},
	{7.0, 4100, -0.930, 7.23, 0.630, 0.64},
	{8.0, 3990, -1.030, 7.40, 0.615, 0.62},
	{9.0, 3930, -1.070, 7.49, 0.608, 0.59}
);

static const STPARS
_M_type_main_sequence_star_Table[21]
(
//   SpT  Teff  BCv    Mbol   R_RSun MSun
	{0.0, 3850, -1.15,  7.65, 0.588, 0.570},
	{0.5, 3770, -1.29,  7.91, 0.544, 0.540},
	{1.0, 3660, -1.42,  8.22, 0.501, 0.500},
	{1.5, 3620, -1.50,  8.35, 0.482, 0.470},
	{2.0, 3560, -1.62,  8.59, 0.446, 0.440},
	{2.5, 3470, -1.78,  8.82, 0.421, 0.400},
	{3.0, 3430, -1.93,  9.21, 0.361, 0.370},
	{3.5, 3270, -2.28,  9.82, 0.300, 0.270},
	{4.0, 3210, -2.51, 10.10, 0.274, 0.230},
	{4.5, 3110, -2.84, 10.74, 0.217, 0.184},
	{5.0, 3060, -3.11, 11.04, 0.196, 0.162},
	{5.5, 2930, -3.58, 11.72, 0.156, 0.123},
	{6.0, 2810, -4.13, 12.19, 0.137, 0.102},
	{6.5, 2740, -4.62, 12.48, 0.126, 0.093},
	{7.0, 2680, -4.99, 12.71, 0.120, 0.090},
	{7.5, 2630, -5.32, 12.84, 0.116, 0.088},
	{8.0, 2570, -5.65, 12.95, 0.114, 0.085},
	{8.5, 2420, -5.78, 13.42, 0.104, 0.080},
	{9.0, 2380, -5.86, 13.54, 0.102, 0.079},
	{9.5, 2350, -6.13, 13.67, 0.101, 0.078},
	// Data of L0 star
	{10.0, 2270, -6.25, 13.75, 0.102, 0.077}
);

//===========================================================================//
//                                                                           //
//                         Wolf-Rayet star tables                            //
//                                                                           //
//===========================================================================//
// Reference:
// Sander, A. A. C.; Hamann, W. -R.; Todt, H.; Hainich, R.; Shenar, T.; Ramachandran, V.; Oskinova, L. M. (2019). 
// "The Galactic WC and WO stars. The impact of revised distances from Gaia DR2 and their role as massive black hole progenitors".
// https://doi.org/10.1051%2F0004-6361%2F201833712
// And luminosities are converted to BCv and MBol

static const STPARS
_WN_Wolf_Rayet_star_Table[7]
(
//   SpT  Teff    BCv     Mbol    R_RSun MSun
	{2.0, 141000, -6.277, -8.877, 0.89, 16},
	{3.0,  85000, -5.416, -8.616, 2.30, 19},
	{4.0,  70000, -4.713, -8.513, 2.30, 15},
	{5.0,  60000, -3.870, -8.270, 3.70, 15},
	{6.0,  56000, -3.170, -8.270, 5.70, 18},
	{7.0,  50000, -3.420, -9.120, 6.00, 21},
	{8.0,  45000, -2.770, -8.270, 6.60, 11}
);

static const STPARS
_WN_Big_Wolf_Rayet_star_Table[5]
(
//   SpT  Teff   BCv     Mbol     R_RSun MSun
	{5.0, 50000, -4.000, -12.000, 20, 200},
	{6.0, 45000, -4.056, -11.556, 25,  74},
	{7.0, 45000, -3.813, -11.013, 23,  52},
	{8.0, 40000, -3.345, -10.545, 22,  39},
	{9.0, 35000, -3.093, -10.193, 23,  33}
);

static const STPARS
_WC_Wolf_Rayet_star_Table[6]
(
//   SpT  Teff    BCv     Mbol    R_RSun MSun
	{4.0, 117000, -4.976, -8.257, 0.9, 10},
	{5.0,  83000, -4.390, -9.260, 3.2, 18},
	{6.0,  78000, -4.760, -9.510, 3.6, 18},
	{7.0,  71000, -4.460, -9.260, 4.0, 17},
	{8.0,  60000, -3.940, -9.260, 6.3, 18},
	{9.0,  44000, -3.189, -8.759, 8.7, 13}
);

static const STPARS
_WO_Wolf_Rayet_star_Table[1]
(
//   SpT  Teff    BCv     Mbol    R_RSun MSun
	{2.0, 200000, -8.058, -9.758, 0.7, 22}
);

//===========================================================================//
//                                                                           //
//                       Sub-stellar Object tables                           //
//                                                                           //
//===========================================================================//
// Reference: Mamajek, Eric. "A Modern Mean Dwarf Stellar Color and Effective Temperature Sequence". 
// University of Rochester, Department of Physics and Astronomy.
// http://www.pas.rochester.edu/~emamajek/EEM_dwarf_UBVIJHK_colors_Teff.txt
// Stars cooler than L5 has no bolometric corrections
// Masses of stars cooler than L2 are calculated with universe sandbox simulation.

static const STPARS
_L_type_Star_Table[10]
(
//   SpT  Teff  BCv    Mbol   R_RSun  MSun
	{0.0, 2270, -6.25, 13.75, 0.1020, 0.077000},
	{1.0, 2160, -6.48, 14.02, 0.0995, 0.076000},
	{2.0, 2060, -6.62, 14.28, 0.0970, 0.075000},
	{3.0, 1920, -7.05, 14.65, 0.0942, 0.074430},
	{4.0, 1870, -7.53, 14.77, 0.0940, 0.074065},
	{5.0, 1710, -7.87, 15.23, 0.0909, 0.072825},
	{6.0, 1550, inf  , 15.70, 0.0891, 0.071520},
	{7.0, 1530, inf  , 15.77, 0.0886, 0.071356},
	{8.0, 1420, inf  , 16.12, 0.0875, 0.070440},
	{9.0, 1370, inf  , 16.27, 0.0877, 0.069980}
);

static const STPARS
_T_type_Star_Table[15]
(
//   SpT  Teff  BCv  Mbol   R_RSun  MSun
	{0.0, 1255, inf, 16.39, 0.0980, 0.06717},
	{1.0, 1240, inf, 16.47, 0.1000, 0.06680},
	{2.0, 1220, inf, 16.57, 0.1000, 0.06628},
	{3.0, 1200, inf, 16.67, 0.1020, 0.06575},
	{4.0, 1180, inf, 16.84, 0.1010, 0.06520},
	{4.5, 1170, inf, 16.99, 0.0990, 0.06493},
	{5.0, 1160, inf, 17.12, 0.1010, 0.06465},
	{5.5, 1040, inf, 17.34, 0.0990, 0.06115},
	{6.0,  950, inf, 17.54, 0.1000, 0.05715},
	{7.0,  825, inf, 18.17, 0.0980, 0.04970},
	{7.5,  750, inf, 18.59, 0.0950, 0.04503},
	{8.0,  680, inf, 19.02, 0.0950, 0.04020},
	{8.5,  600, inf, 19.57, 0.0970, 0.03460},
	{9.0,  560, inf, 20.12, 0.1000, 0.03155},
	{9.5,  510, inf, 20.51, 0.1020, 0.02773} // Universe sandbox gives the radius of 0.0899
);

static const STPARS
_Sub_Brown_Dwarf_Table[6]
(
// These data are calculated by universe sandbox beacuse no real data for these stars except Y0-star.
//   SpT  Teff BCv  Mbol   R_RSun  MSun
	{0.0, 450, inf, 21.04, 0.1040, 0.0257},
	{0.5, 400, inf, 21.51, 0.0925, 0.0210},
	{1.0, 360, inf, 21.91, 0.0944, 0.0177},
	{1.5, 325, inf, 22.32, 0.0963, 0.0146},
	{2.0, 320, inf, 22.38, 0.0965, 0.0142},
	{4.0, 250, inf, 23.42, 0.0977, 0.0124} // Lower limit of a substellar object is 13MJ(~0.0124MSun)
);

_RAND_END


const STPARS* GetStarTable(LSTARCLS::SpecClass Class, size_t* Tyc)
{
	switch (Class)
	{
	case cse::LSTARCLS::O:
	case cse::LSTARCLS::Of_p:
		*Tyc = 13;
		return _RAND _O_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::B:
		*Tyc = 14;
		return _RAND _B_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::A:
		*Tyc = 10;
		return _RAND _A_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::F:
		*Tyc = 11;
		return _RAND _F_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::G:
		*Tyc = 10;
		return _RAND _G_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::K:
		*Tyc = 10;
		return _RAND _K_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::M:
		*Tyc = 20;
		return _RAND _M_type_main_sequence_star_Table;
		break;
	case cse::LSTARCLS::WN:
	case cse::LSTARCLS::WNE:
	case cse::LSTARCLS::WNL:
	case cse::LSTARCLS::Ofpe:
	case cse::LSTARCLS::WNC:
		*Tyc = 7;
		return _RAND _WN_Wolf_Rayet_star_Table;
		break;
	case cse::LSTARCLS::WNh:
	case cse::LSTARCLS::WNha:
		*Tyc = 5;
		return _RAND _WN_Big_Wolf_Rayet_star_Table;
		break;
	case cse::LSTARCLS::WC:
	case cse::LSTARCLS::WCE:
	case cse::LSTARCLS::WCL:
	case cse::LSTARCLS::WCd:
		*Tyc = 6;
		return _RAND _WC_Wolf_Rayet_star_Table;
		break;
	case cse::LSTARCLS::WO:
		*Tyc = 1;
		return _RAND _WO_Wolf_Rayet_star_Table;
		break;
	case cse::LSTARCLS::L:
		*Tyc = 10;
		return _RAND _L_type_Star_Table;
		break;
	case cse::LSTARCLS::T:
		*Tyc = 15;
		return _RAND _T_type_Star_Table;
		break;
	case cse::LSTARCLS::Y:
		*Tyc = 5;
		return _RAND _Sub_Brown_Dwarf_Table;
		break;
	/*
	case cse::LSTARCLS::C_R:
		break;
	case cse::LSTARCLS::C_N:
		break;
	case cse::LSTARCLS::C_J:
		break;
	case cse::LSTARCLS::C_H:
		break;
	case cse::LSTARCLS::C_Hd:
		break;
	case cse::LSTARCLS::S:
		break;
	case cse::LSTARCLS::MS:
		break;
	case cse::LSTARCLS::SC:
		break;
	case cse::LSTARCLS::DA:
	case cse::LSTARCLS::DB:
	case cse::LSTARCLS::DO:
	case cse::LSTARCLS::DQ:
	case cse::LSTARCLS::DZ:
	case cse::LSTARCLS::DC:
	case cse::LSTARCLS::DX:
		break;
	case cse::LSTARCLS::D:
		break;
	case cse::LSTARCLS::Q:
		break;
	case cse::LSTARCLS::X:
		break;
	case cse::LSTARCLS::Exotic:
		break;
	*/
	default:
		*Tyc = 0;
		return nullptr;
		break;
	}
}

_CSE_END