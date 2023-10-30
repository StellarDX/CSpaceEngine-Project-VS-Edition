#pragma once

#ifndef __STARBARYCENTER__
#define __STARBARYCENTER__

#include "CSE/Core/CSECore.h"
#include "CSE/Object.h"
#include "CSE/SCStream.h"

_CSE_BEGIN

////////////////////////////////////////////////////////////
//                Coordinate definations                  //
////////////////////////////////////////////////////////////

struct _Coordinate24_Base
{
    float64  sec;
    uint16_t min;
    uint16_t hrs;
};

struct _Coordinate360_Base
{
    float64  sec;
    uint16_t min;
    uint16_t deg;
};

struct _Coordinate90_Base
{
    float64  sec;
    uint16_t min;
    uint16_t deg;
    bool     neg;
};

union UCoordinate24;
union Coordinate90;
union UCoordinate360;

union UCoordinate24
{
    _Coordinate24_Base _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    UCoordinate24() {};
    UCoordinate24(float64 h);
    UCoordinate24(uint64 h, uint64 m, float64 s);

    bool neg()const { return 0; }
    uint64 hrs()const;
    uint64 deg()const { return hrs(); }
    uint64 min()const;
    float64 sec()const;

    operator float64();
    UCoordinate24 operator=(float64 h);
};

union Coordinate90
{
    _Coordinate90_Base _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    Coordinate90() {};
    Coordinate90(float64 d);
    Coordinate90(int64 d, uint64 m, float64 s);

    bool neg()const;
    uint64 deg()const;
    uint64 min()const;
    float64 sec()const;

    operator float64();
    Coordinate90 operator=(float64 h);
};

union UCoordinate360
{
    _Coordinate360_Base _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    UCoordinate360() {};
    UCoordinate360(float64 d);
    UCoordinate360(uint64 d, uint64 m, float64 s);

    bool neg()const { return 0; }
    uint64 deg()const;
    uint64 min()const;
    float64 sec()const;

    operator float64();
    UCoordinate360 operator=(float64 d);
};

////////////////////////////////////////////////////////////
//                       OPERATORS                        //
////////////////////////////////////////////////////////////

_STD ostream& operator<<(_STD ostream& os, UCoordinate24 Coord);
_STD ostream& operator<<(_STD ostream& os, Coordinate90 Coord);
_STD ostream& operator<<(_STD ostream& os, UCoordinate360 Coord);

////////////////////////////////////////////////////////////
//                    StarBarycenter                      //
////////////////////////////////////////////////////////////

using StarBarycenter = 
class Location
{
public:
    _STD vector<_STD string> Name;
    _STD string CenterOf = NO_DATA_STRING;

    UCoordinate24 RA;
    Coordinate90 Dec;
    float64 Dist; // Distance in parsec

    Location() {}
    Location(vec3 P) : RA(P.x), Dec(P.y), Dist(P.z) {}
    Location(UCoordinate24 alf, Coordinate90 del, float64 Ds = 0) : RA(alf), Dec(del), Dist(Ds) {}

    operator vec3();
};

class StarLocation : virtual public Location
{
public:
    _STD shared_ptr<Object> Pointer = nullptr;
    Object GetSEObject()const { return *Pointer; }

    _NODISCARD Object& operator*() const noexcept;
    _NODISCARD Object* operator->() const noexcept;
};

template<typename _Ty> requires std::is_same_v<_Ty, int>
class _StarBarycen_Base : public __OSC_BASE<_Ty>
{
public:
    enum _Fmtflags
    {
        // constants for formatting options
        _Fmtmask = 0b11111111111111111111111111111111,
        _Fmtzero = 0b00000000000000000000000000000000
    };

    static constexpr _Fmtflags DisableWM  = static_cast<_Fmtflags>(0b10000000000000000000000000000000);
    static constexpr _Fmtflags Fixed      = static_cast<_Fmtflags>(0b01000000000000000000000000000000);
    static constexpr _Fmtflags FltCoord   = static_cast<_Fmtflags>(0b00100000000000000000000000000000);

    static constexpr _Fmtflags Default    = _Fmtzero;

    _Ty __CLR_OR_THIS_CALL _BaseInit()
    {
        return Default;
    }

    bool PecularKey(_STD string Key, uint64* Mode)const noexcept
    {
        return 0; // nothing
    }
};

_SC_BEGIN

class starbarycen_ostream : public _SC _Smart_Output_Base<_CSE _StarBarycen_Base<int>>
{
    _STD ostream& fout;
public:
    explicit __CLR_OR_THIS_CALL starbarycen_ostream(_STD ostream& _Os) : fout(_Os)
    {
        _Init();
    }

    //friend table::KeyValue MakeTable(object_ostream& _Os, _CSE Object Obj);

    _STD string str()const
    {
        return _Write(_Buf, false, 0Ui64);
    }

    void write()
    {
        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] Generating SpaceEngine Program... ", OLogLevel, true);
        _STD string _Final = _Write(_Buf, false, 0Ui64);
        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] DONE", OLogLevel, true);

        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] Preparing to write file.", OLogLevel, true);

        int len = MultiByteToWideChar(CP_ACP, 0, _Final.c_str(), -1, NULL, 0);
        wchar_t* wstr = new wchar_t[len + 1];
        memset(wstr, 0, len + 1);

        MultiByteToWideChar(CP_ACP, 0, _Final.c_str(), -1, wstr, len);

        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] Using file encoding : " + _STD to_string(encod()), OLogLevel, true);

        len = WideCharToMultiByte(encod(), 0, wstr, -1, NULL, 0, NULL, NULL);
        char* str = new char[len + 1];
        memset(str, 0, len + 1);

        WideCharToMultiByte(encod(), 0, wstr, -1, str, len, NULL, NULL);

        if (wstr) { delete[] wstr; }
        _STD string ret = str;
        if (str) { delete[] str; }

        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] Writing file...", OLogLevel, true);

        fout << ret;

        Log_OS.Out("OSCStream", "INFO", "[CSE Smart Output] DONE", OLogLevel, true);
    }

    starbarycen_ostream& operator<<(starbarycen_ostream& (_cdecl* _Manip)(starbarycen_ostream&))
    { // insert by calling function with output stream and argument
        (*_Manip)(*this);
        return *this;
    }
};

_SC_END

using OBarycenterStream = _SC starbarycen_ostream;

StarBarycenter BarycenterLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it, Object* StarDetails = nullptr);
StarLocation StarLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it);
StarBarycenter GetBarycenter(ISCStream _Is, _STD string _Name, Object* StarDetails = nullptr);
StarLocation GetSEStar(ISCStream _Is, _STD string _Name);

OBarycenterStream& operator<<(OBarycenterStream& _Os, _CSE StarBarycenter _Obj);
OBarycenterStream& operator<<(OBarycenterStream& _Os, _CSE StarLocation _Obj);

inline OBarycenterStream& __CLRCALL_OR_CDECL Fixed(OBarycenterStream& _Os)
{
    _Os.setf(OBarycenterStream::Fixed);
    return _Os;
}

inline OBarycenterStream& __CLRCALL_OR_CDECL NoWaterMarks(OBarycenterStream& _Os)
{
    _Os.setf(OBarycenterStream::DisableWM);
    return _Os;
}

inline OBarycenterStream& __CLRCALL_OR_CDECL CoordAsFloat(OBarycenterStream& _Os)
{
    _Os.setf(OBarycenterStream::FltCoord);
    return _Os;
}

inline OBarycenterStream& __CLRCALL_OR_CDECL Resetf(OBarycenterStream& _Os)
{
    _Os.flags(OBarycenterStream::Default);
    return _Os;
}

_CSE_END

#endif