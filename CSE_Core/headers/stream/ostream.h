////////////////////////////////////////////////////////
//                  SC Output Module                  //
// -------------------------------------------------- //
// Output Celestial object data to .sc file           //
////////////////////////////////////////////////////////

#pragma once

#ifndef __CSE_OSTREAM__
#define __CSE_OSTREAM__

#include <ostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "..\Core\CSECore.h"
#include "..\Core\CelObject.h"

_CSE_BEGIN

namespace sc
{
	typedef std::vector<Object> Buffer;
}

_SC_BEGIN
class OSCManip // Output manipulator
{
public:
	enum _Fmtflags // constants for formatting options
	{
		_Fmtmask = 0b11111111111111111111111111111111,
		_Fmtzero = 0b00000000000000000000000000000000
	};
	// Tags
	static constexpr _Fmtflags Physics        = static_cast<_Fmtflags>(0b00000000000000100000000000000000);
	static constexpr _Fmtflags Optical        = static_cast<_Fmtflags>(0b00000000000000010000000000000000);
	static constexpr _Fmtflags Life           = static_cast<_Fmtflags>(0b00000000000000000000010000000000);
	static constexpr _Fmtflags Interior       = static_cast<_Fmtflags>(0b00000000000000000000001000000000);
	static constexpr _Fmtflags Surface        = static_cast<_Fmtflags>(0b00000000000000000000000100000000);
	static constexpr _Fmtflags Ocean          = static_cast<_Fmtflags>(0b00000000000000000000000010000000);
	static constexpr _Fmtflags Clouds         = static_cast<_Fmtflags>(0b00000000000000000000000001000000);
	static constexpr _Fmtflags Atmosphere     = static_cast<_Fmtflags>(0b00000000000000000000000000100000);
	static constexpr _Fmtflags Aurora         = static_cast<_Fmtflags>(0b00000000000000000000000000010000);
	static constexpr _Fmtflags Rings          = static_cast<_Fmtflags>(0b00000000000000000000000000001000);
	static constexpr _Fmtflags AccDisk        = static_cast<_Fmtflags>(0b00000000000000000000000000000100);
	static constexpr _Fmtflags Corona         = static_cast<_Fmtflags>(0b00000000000000000000000000000010);
	static constexpr _Fmtflags CometTail      = static_cast<_Fmtflags>(0b00000000000000000000000000000001);
	// Tag Options
	static constexpr _Fmtflags RotationAuto   = static_cast<_Fmtflags>(0b00000000000000000000000000000000);
	static constexpr _Fmtflags RotationSimple = static_cast<_Fmtflags>(0b00000000000000000100000000000000);
	static constexpr _Fmtflags RotationIAU    = static_cast<_Fmtflags>(0b00000000000000001000000000000000);
	static constexpr _Fmtflags RotationMoonDE = static_cast<_Fmtflags>(0b00000000000000001100000000000000);
	static constexpr _Fmtflags OrbitAuto      = static_cast<_Fmtflags>(0b00000000000000000000000000000000);
	static constexpr _Fmtflags OrbitDefault   = static_cast<_Fmtflags>(0b00000000000000000011100000000000);
	static constexpr _Fmtflags OrbitBinary    = static_cast<_Fmtflags>(0b00000000000000000000100000000000);
	static constexpr _Fmtflags OrbitPlanet    = static_cast<_Fmtflags>(0b00000000000000000001000000000000);
	static constexpr _Fmtflags OrbitComet     = static_cast<_Fmtflags>(0b00000000000000000001100000000000);
	static constexpr _Fmtflags OrbitTLE       = static_cast<_Fmtflags>(0b00000000000000000010000000000000);
	// Output Presets
	static constexpr _Fmtflags Simple         = static_cast<_Fmtflags>(0b10000000000000000000000000000000);
	static constexpr _Fmtflags Detail         = static_cast<_Fmtflags>(0b10100000000000110000011111111111);
	// Others
	static constexpr _Fmtflags Watermark      = static_cast<_Fmtflags>(0b10000000000000000000000000000000);
	static constexpr _Fmtflags Fixed          = static_cast<_Fmtflags>(0b01000000000000000000000000000000);
	static constexpr _Fmtflags Booleans       = static_cast<_Fmtflags>(0b00100000000000000000000000000000);
	static constexpr _Fmtflags ForceTidalLock = static_cast<_Fmtflags>(0b00010000000000000000000000000000);
};

_SC_END

class OSCStream : virtual public _SC OSCManip
{
	using fmtflags = int;

	fmtflags _Fmtfl = Simple;
	std::ostream& Output;
	_SC Buffer Catalog;

	//template<typename genType>
	inline void WriteKeyValue(std::string Key, float64 Value, int Indent = 1)
	{
		if (std::isinf(Value)) { return; }
		for (size_t i = 0; i < Indent; i++){Output << "\t";}
		Output << Key << Value << '\n';
	}

	inline void WriteKeyValue(std::string Key, vec3 Value, int Indent = 1)
	{
		if (std::isinf(Value.x) || std::isinf(Value.y) || std::isinf(Value.z)) { return; }
		for (size_t i = 0; i < Indent; i++) { Output << "\t"; }
		Output << Key << Value << '\n';
	}

	inline void WriteKeyValue(std::string Key, std::string Value, int Indent = 1)
	{
		if (Value == "None") { return; }
		for (size_t i = 0; i < Indent; i++) { Output << "\t"; }
		Output << Key << '\"' << Value << '\"' << '\n';
	}

	const int BoolKey = 29;
	inline void WriteKeyValue(std::string Key, bool Value, int Indent = 1)
	{
		if (Value == false || !(_Fmtfl & (1 << BoolKey))) { return; }
		for (size_t i = 0; i < Indent; i++) { Output << "\t"; }
		Output << Key << "true" << '\n';
	}

	void WaterMark();
	void Physical(Object Obj);
	void Optical(Object Obj);
	void Rotation(Object Obj);
	void Orbit(Object Obj);

	void Life(Object Obj);
	void Interior(Object Obj);
	void Surface(Landscape Surf);
	void Ocean(OceanParam Sea);
	void Clouds(CloudsParam Cld);
	void Atmosphere(AtmParam Atm);
	void Aurora(AurParam Aur);
	void Rings(RingsParam Rng);
	void AccDisk(AccDiskParam ADK);
	void Corona(CoronaParam Crn);
	void CometTail(CometTailParam Com);

public:
	static constexpr unsigned BasicDataPrecision = 5;
	static constexpr unsigned RotationPrecision  = 5;
	static constexpr unsigned OrbitPrecision     = 9;
	static constexpr unsigned InteriorPrecision  = 5;
	static constexpr unsigned SurfacePrecision   = 5;
	static constexpr unsigned OceanPrecision     = 5;
	static constexpr unsigned CloudsPrecision    = 5;
	static constexpr unsigned AtmPrecision       = 5;
	static constexpr unsigned AuroraPrecision    = 5;
	static constexpr unsigned RingsPrecision     = 5;
	static constexpr unsigned AccDiskPrecision   = 5;
	static constexpr unsigned CoronaPrecision    = 5;
	static constexpr unsigned CometTailPrecision = 5;
	
public:
	OSCStream(std::ostream& os) : Output(os){}
	OSCStream& operator=(const OSCStream& os) = delete;

	friend OSCStream& operator<<(OSCStream& os, const Object& Obj);
	OSCStream& /*__CLR_OR_THIS_CALL*/ operator<<(OSCStream& (__cdecl* f)(OSCStream&));
	fmtflags flags()const { return _Fmtfl; };
	fmtflags flags(fmtflags _Newfmtflags); // set format flags to argument
	fmtflags setf(fmtflags _Newfmtflags); // merge in format flags argument
	void unsetf(fmtflags _Mask); // clear format flags under mask argument

	void Write();
};

// Functions for setting format

inline OSCStream& /*__CLRCALL_OR_CDECL*/ Simple(OSCStream& os)
{
	os.flags(_SC OSCManip::Simple);
	return os;
}

inline OSCStream& /*__CLRCALL_OR_CDECL*/ Detail(OSCStream& os)
{
	os.flags(_SC OSCManip::Detail);
	return os;
}

inline OSCStream& /*__CLRCALL_OR_CDECL*/ NoWaterMark(OSCStream& os)
{
	os.unsetf(_SC OSCManip::Watermark);
	return os;
}

inline OSCStream& /*__CLRCALL_OR_CDECL*/ Fixed(OSCStream& os)
{
	os.setf(_SC OSCManip::Fixed);
	return os;
}

inline OSCStream& /*__CLRCALL_OR_CDECL*/ IgnoreBooleans(OSCStream& os)
{
	os.unsetf(_SC OSCManip::Booleans);
	return os;
}

// If TidalLocked is true, ignore all rotation elements for objects, force tidal locking to parent objects
inline OSCStream& /*__CLRCALL_OR_CDECL*/ IgnoreObjsRotationIAU(OSCStream& os)
{
	os.setf(_SC OSCManip::ForceTidalLock);
	return os;
}



_CSE_END

#endif