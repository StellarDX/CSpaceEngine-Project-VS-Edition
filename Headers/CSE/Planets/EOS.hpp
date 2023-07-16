#pragma once

#ifndef _EOS_
#define _EOS_

#include "CSE/Core.h"

// Equations of state for planet models

_CSE_BEGIN

// Rose-Vinet equation of state
// https://en.wikipedia.org/wiki/Rose%E2%80%93Vinet_equation_of_state
struct ROSE_VINET_EOS
{
    ROSE_VINET_EOS(float64 _Kx0, float64 _Kx1, float64 _Rx0) : _K0(_Kx0), _K1(_Kx1), _Rho0(_Rx0) {}

    float64 K0()const { return _K0; }
    float64 K1()const { return _K1; }
    float64 Rho0()const { return _Rho0; }

    float64 Pressure(float64 Density)
    {
        float64 Eta = Density / _Rho0;
        return 3. * _K0 * pow(Eta, 2. / 3.) * (1. - pow(Eta, -(1. / 3.))) * exp((3. / 2.) * (_K1 - 1.) * (1. - pow(Eta, -(1. / 3.))));
    }

    float64 dPressure(float64 Density) // derivate function
    {
        float64 Eta = Density / _Rho0;
        float64 u0 = exp((3. / 2.) * (_K1 - 1.) * (1. - pow(Eta, -(1. / 3.))));
        float64 v0 = (_Rho0 / (cbrt(Eta) * Density)) / _Rho0;
        return _K0 * pow(Eta, 2. / 3.) * u0 * v0 + 2. * _K0 * _Rho0 * pow(Eta, 2. / 3.) * u0 * ((1. - pow(Eta, -(1. / 3.))) / (_Rho0 * Density)) + (3. / 2.) * _K0 * pow(Eta, 2. / 3.) * u0 * (_K1 - 1.) * (1. - pow(Eta, -(1. / 3.))) * v0;
    }

private:
    float64 _K0;
    float64 _K1;
    float64 _Rho0;
};

// Birch-Murnaghan equation of state
// https://en.wikipedia.org/wiki/Birch%E2%80%93Murnaghan_equation_of_state
struct BIRCH_MURNAGHAN_EOS
{
    BIRCH_MURNAGHAN_EOS(float64 _Kx0, float64 _Kx1, float64 _Rx0) : _K0(_Kx0), _K1(_Kx1), _Rho0(_Rx0) {}

    float64 K0()const { return _K0; }
    float64 K1()const { return _K1; }
    float64 Rho0()const { return _Rho0; }

    float64 Pressure(float64 Density)
    {
        float64 Eta = Density / _Rho0;
        return (3. / 2.) * _K0 * (pow(Eta, 7. / 3.) - pow(Eta, 5. / 3.)) * (1. + (3. / 4.) * (_K1 - 4.) * (pow(Eta, 2. / 3.) - 1.));
    }

    friend struct BIRCH_MURNAGHAN_4TH_EOS;

private:
    float64 _K0;
    float64 _K1;
    float64 _Rho0;
};
#if 0
struct BIRCH_MURNAGHAN_4TH_EOS : public BIRCH_MURNAGHAN_EOS
{
    using _Mybase = BIRCH_MURNAGHAN_EOS;

    BIRCH_MURNAGHAN_4TH_EOS(float64 _Kx0, float64 _Kx1, float64 _Kx2, float64 _Rx0) : _Mybase(_Kx0, _Kx1, _Rx0), _K2(_Kx2) {}

    float64 K2()const { return _K2; }

    float64 Pressure(float64 Density)
    {
        float64 Eta = Density / _Rho0;
        return _Mybase::Pressure(Density) + (3. / 2.) * _K0 * (pow(Eta, 7. / 3.) - pow(Eta, 5. / 3.)) * (3. / 8.) * _K0 * pow(pow(Eta, 2. / 3.) - 1., 2) * (_K0 * _K2 + _K1 * (_K1 - 7.) + (143. / 9));
    }

private:
    float64 _K2;
};
#endif
static const float64 TFD_GAMMA_TABLE[] =
{
  +1.512E-2, +8.955E-2, +1.090E-1, +5.089E+0, -5.980E+0,
  +2.181E-3, -4.015E-1, +1.698E+0, -9.566E+0, +9.873E+0,
  -3.328E-4, +5.167E-1, -2.369E+0, +1.349E+1, -1.427E+1,
  -1.384E-2, -6.520E-1, +3.529E+0, -2.095E+1, +2.264E+1
};

// Thomas-Fermi-Dirac method for huge-pressure
// In very huge pressure(~1E13 dyne/cm^2, ~1TPa), atoms will be cracked.
// So matter's pressure-density relation will only affect by relative atomic mass and atomic number.
// But this method is not accurate in normal pressure because of crystallizations and chemical bonds.
// Reference: E. E. SALPZTXR AND H.S.ZAPOISKV (Received 9 February 1967)
// "Theoretical High-Pressure Equations of State including Correlation Energy"
// https://lweb.cfa.harvard.edu/~lzeng/papers/salpeter_zapolsky_1967.pdf
struct THOMAS_FERMI_DIRAC_EOS
{
    THOMAS_FERMI_DIRAC_EOS(float64 _Ax0, float64 _Zx0) : AtomicWeight(_Ax0), NProton(_Zx0) {}

    float64 A()const { return AtomicWeight; }
    float64 Z()const { return NProton; }

    // Unit of the input value is dyne/cm^2, which equals 0.1 pa.
    // and return value is g/cm^3, which equals 1000 Kg/m^3
    float64 Density(float64 Pressure)
    {
        float64 eps = cbrt(3. / (32. * pow(CSE_PI, 2) * pow(NProton, 2)));
        float64 phi = cbrt(3.) / 20. + eps / 4;

        float64 x00 = 1. / (8.884E-3 + sqrt(eps) * 4.988E-1 + eps * 5.2604E-1);

        float64 alf = 1. / (1.941E-2 - sqrt(eps) * 6.277E-2 + eps * 1.076);
        float64 bet[6];
        bet[0] = x00 * phi - 1;
        bet[1] = bet[0] * alf + ((1. + bet[0]) / phi);
        const float64* gam = TFD_GAMMA_TABLE;
        for (size_t i = 2; i <= 5; i++)
        {
            size_t j = (i - 2) * 5;
            bet[i] = 1. / pow(gam[j] + gam[j + 1] * pow(eps, 1. / 2.) + gam[j + 2] * eps + gam[j + 3] * pow(eps, 3. / 2.) + gam[j + 4] * pow(eps, 2), float64(i));
        }

        float64 ksi = pow(Pressure / 9.524E+13, 1. / 5.) * pow(NProton, -2. / 3.);
        float64 x0ksi = (1. + exp(-alf * ksi) * (bet[0] + bet[1] * ksi + bet[2] * pow(ksi, 2) + bet[3] * pow(ksi, 3) + bet[4] * pow(ksi, 4) + bet[5] * pow(ksi, 5))) / (ksi + phi);
        return 3.886 * (AtomicWeight * NProton) / pow(x0ksi, 3);
    }

private:
    float64 AtomicWeight;
    float64 NProton;
};

// Fast exponential EOS model
// Reference: S. Seager, M. Kuchner, C. A. Hier-Majumder, and B. Militzer
// "MASS-RADIUS RELATIONSHIPS FOR SOLID EXOPLANETS"
// https://arxiv.org/abs/0707.2895
struct FAST_POLYTROPIC_EOS
{
    FAST_POLYTROPIC_EOS(float64 Rx0, float64 Cx0, float64 Px0) : _Rho0(Rx0), _Cx0(Cx0), _Power(Px0) {}

    float64 Rho0()const { return _Rho0; }
    float64 C()const { return _Cx0; }
    float64 n()const { return _Power; }

    float64 Density(float64 Pressure)
    {
        return _Rho0 + _Cx0 * pow(Pressure, _Power);
    }

private:
    float64 _Rho0;
    float64 _Cx0;
    float64 _Power;
};

// ---------- Parameters for the Vinet ( V ) or Birch-Murnagham (BME) EOS Fits ---------- //
extern BIRCH_MURNAGHAN_EOS Carbon_BME_Graphite;
extern BIRCH_MURNAGHAN_EOS Ferrum_BME_Alpha;
extern ROSE_VINET_EOS      Ferrum_Vinet_Epsilon;
extern BIRCH_MURNAGHAN_EOS FeS_BME;
extern BIRCH_MURNAGHAN_EOS H2O_BME_IceVII;
extern BIRCH_MURNAGHAN_EOS MgO_BME;
extern BIRCH_MURNAGHAN_EOS MgSiO3_BME_en;
//extern BIRCH_MURNAGHAN_4TH_EOS MgSiO3_BME4_pv;
extern BIRCH_MURNAGHAN_EOS MgFeSiO3_BME_pv;
extern BIRCH_MURNAGHAN_EOS SiC_BME;

// ---------- Fits to the Merged Vinet/BME and TFD EOS of the Form Rho(P) = Rho0 + c * P^n ---------- //
extern FAST_POLYTROPIC_EOS Ferrum_Exponential_Epsilon;
extern FAST_POLYTROPIC_EOS MgSiO3_Exponential_Perovskite;
extern FAST_POLYTROPIC_EOS MgFeSiO3_Exponential;
extern FAST_POLYTROPIC_EOS H2O_Exponential_IceVII;
extern FAST_POLYTROPIC_EOS Carbon_Exponential_Graphite;
extern FAST_POLYTROPIC_EOS SiC_Exponential;

extern THOMAS_FERMI_DIRAC_EOS Bismuth_TFD;
extern THOMAS_FERMI_DIRAC_EOS Plumbum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Aurum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Galena_TFD;
extern THOMAS_FERMI_DIRAC_EOS Stannum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Cadmium_TFD;
extern THOMAS_FERMI_DIRAC_EOS Argentum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Zinc_TFD;
extern THOMAS_FERMI_DIRAC_EOS Cuprum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Nickel_TFD;
extern THOMAS_FERMI_DIRAC_EOS Ferrum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Sphalerite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Troilite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Pyrite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Wustite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Oldhamite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Magnetite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Hematite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Fayalite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Rutile_TFD;
extern THOMAS_FERMI_DIRAC_EOS Andratite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Pyrope_TFD;
extern THOMAS_FERMI_DIRAC_EOS Grossularite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Diopside_TFD;
extern THOMAS_FERMI_DIRAC_EOS Anorthite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Orthoclase_TFD;
extern THOMAS_FERMI_DIRAC_EOS Corundum_TFD;
extern THOMAS_FERMI_DIRAC_EOS Spinel_TFD;
extern THOMAS_FERMI_DIRAC_EOS Jadeite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Albite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Quartz_TFD;
extern THOMAS_FERMI_DIRAC_EOS Forsterite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Enstatite_TFD;
extern THOMAS_FERMI_DIRAC_EOS Periclase_TFD;
extern THOMAS_FERMI_DIRAC_EOS Spodumene_TFD;
extern THOMAS_FERMI_DIRAC_EOS Beryl_TFD;

_CSE_END

#endif