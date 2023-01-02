#include "CSE/Planets/EOS.hpp"

_CSE_BEGIN

// ---------- Parameters for the Vinet ( V ) or Birch-Murnagham (BME) EOS Fits ---------- //
// Fit              Atom or Compound      K0       K1   Rho0
BIRCH_MURNAGHAN_EOS Carbon_BME_Graphite  ( 33.8E9, 8.90, 2.25E3); //
BIRCH_MURNAGHAN_EOS Ferrum_BME_Alpha     (162.5E9, 5.50, 7.86E3);
ROSE_VINET_EOS      Ferrum_Vinet_Epsilon (156.2E9, 6.08, 8.30E3);
BIRCH_MURNAGHAN_EOS FeS_BME              ( 35.0E9, 5.00, 4.77E3);
BIRCH_MURNAGHAN_EOS H2O_BME_IceVII       ( 23.7E9, 4.15, 1.46E3);
BIRCH_MURNAGHAN_EOS MgO_BME              (177.0E9, 4.00, 3.56E3);
BIRCH_MURNAGHAN_EOS MgSiO3_BME_en        (125.0E9, 5.00, 3.22E3);
//BIRCH_MURNAGHAN_4TH_EOS MgSiO3_BME4_pv   (247.0E9, 3.97, -0.016E-9, 4.10E3);
BIRCH_MURNAGHAN_EOS MgFeSiO3_BME_pv      (266.0E9, 3.90, 4.26E3); // 22MgO.3FeO.25SiO2
BIRCH_MURNAGHAN_EOS SiC_BME              (227.0E9, 4.10, 3.22E3);

// ---------- Fits to the Merged Vinet/BME and TFD EOS of the Form Rho(P) = Rho0 + c * P^n ---------- //
// These fits are valid for the pressure range P < 1E+16 Pa
// Some typos are found in original paper, the following codes corrected them.
//                  Material                       Rho0  c        n
FAST_POLYTROPIC_EOS Ferrum_Exponential_Epsilon    (8300, 0.00349, 0.528); // 
FAST_POLYTROPIC_EOS MgSiO3_Exponential_Perovskite (4100, 0.00161, 0.541);
FAST_POLYTROPIC_EOS MgFeSiO3_Exponential          (4260, 0.00127, 0.549);
FAST_POLYTROPIC_EOS H2O_Exponential_IceVII        (1460, 0.00311, 0.513);
FAST_POLYTROPIC_EOS Carbon_Exponential_Graphite   (2250, 0.00350, 0.514);
FAST_POLYTROPIC_EOS SiC_Exponential               (3220, 0.00172, 0.537);

// ---------- Parameters for the TFD Model ---------- //
#if _ENABLE_TFD_EOS // TODO...
//                     Mineral or element  A       Z          Note
THOMAS_FERMI_DIRAC_EOS Bismuth_TFD        (209.00, 83.00); // Bi
THOMAS_FERMI_DIRAC_EOS Plumbum_TFD        (207.00, 82.00); // Pb
THOMAS_FERMI_DIRAC_EOS Aurum_TFD          (197.00, 79.00); // Au
THOMAS_FERMI_DIRAC_EOS Galena_TFD         (119.50, 49.00); // PbS
THOMAS_FERMI_DIRAC_EOS Stannum_TFD        (119.00, 50.00); // Sn
THOMAS_FERMI_DIRAC_EOS Cadmium_TFD        (112.00, 48.00); // Cd
THOMAS_FERMI_DIRAC_EOS Argentum_TFD       (108.00, 47.00); // Ag
THOMAS_FERMI_DIRAC_EOS Zinc_TFD           ( 65.00, 30.00); // Zn
THOMAS_FERMI_DIRAC_EOS Cuprum_TFD         ( 64.00, 29.00); // Cu
THOMAS_FERMI_DIRAC_EOS Nickel_TFD         ( 59.00, 28.00); // Ni
THOMAS_FERMI_DIRAC_EOS Ferrum_TFD         ( 56.00, 26.00); // Fe
THOMAS_FERMI_DIRAC_EOS Sphalerite_TFD     ( 48.50, 23.00); // ZnS
THOMAS_FERMI_DIRAC_EOS Troilite_TFD       ( 44.00, 21.00); // FeS
THOMAS_FERMI_DIRAC_EOS Pyrite_TFD         ( 38.00, 16.50); // FeS2
THOMAS_FERMI_DIRAC_EOS Wustite_TFD        ( 36.00, 17.00); // FeO
THOMAS_FERMI_DIRAC_EOS Oldhamite_TFD      ( 36.00, 18.00); // CaS
THOMAS_FERMI_DIRAC_EOS Magnetite_TFD      ( 33.12, 15.70); // Fe3O4
THOMAS_FERMI_DIRAC_EOS Hematite_TFD       ( 32.00, 15.20); // Fe2O3
THOMAS_FERMI_DIRAC_EOS Fayalite_TFD       ( 29.16, 14.01); // 2FeO.SiO2
THOMAS_FERMI_DIRAC_EOS Rutile_TFD         ( 26.66, 12.66); // TiO2
THOMAS_FERMI_DIRAC_EOS Andratite_TFD      ( 25.40, 12.50); // 3CaO.Fe2O3.3SiO2
THOMAS_FERMI_DIRAC_EOS Pyrope_TFD         ( 25.01, 12.15); // 3MgO.Al2O3.3SiO2
THOMAS_FERMI_DIRAC_EOS Grossularite_TFD   ( 22.50, 11.20); // 3CaO.Al2O3.3SiO2
THOMAS_FERMI_DIRAC_EOS Diopside_TFD       ( 21.60, 10.80); // MgO.CaO.2SiO2
THOMAS_FERMI_DIRAC_EOS Anorthite_TFD      ( 21.39, 10.62); // CaO.Al2O3.2SiO2
THOMAS_FERMI_DIRAC_EOS Orthoclase_TFD     ( 21.39, 10.62); // K2O.Al2O3.6SiO2
THOMAS_FERMI_DIRAC_EOS Corundum_TFD       ( 20.40, 10.00); // Al2O3
THOMAS_FERMI_DIRAC_EOS Spinel_TFD         ( 20.29, 10.00); // MgO.Al2O3
THOMAS_FERMI_DIRAC_EOS Jadeite_TFD        ( 20.20, 10.00); // Na2O.Al2O3.4SiO2
THOMAS_FERMI_DIRAC_EOS Albite_TFD         ( 20.16, 10.00); // Na2O.Al2O3.6SiO2
THOMAS_FERMI_DIRAC_EOS Quartz_TFD         ( 20.00, 10.00); // SiO2
THOMAS_FERMI_DIRAC_EOS Forsterite_TFD     ( 20.00, 10.00); // 2MgO.SiO2
THOMAS_FERMI_DIRAC_EOS Enstatite_TFD      ( 20.00, 10.00); // MgO.SiO2
THOMAS_FERMI_DIRAC_EOS Periclase_TFD      ( 20.00, 10.00); // MgO
THOMAS_FERMI_DIRAC_EOS Spodumene_TFD      ( 18.60,  9.20); // Li2O.Al2O3.4SiO2
THOMAS_FERMI_DIRAC_EOS Beryl_TFD          ( 18.52,  9.17); // 3BeO.Al2O3.6SiO2
#endif

_CSE_END