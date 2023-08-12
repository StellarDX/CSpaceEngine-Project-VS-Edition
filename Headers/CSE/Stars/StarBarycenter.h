#pragma once

#ifndef __STARBARYCENTER__
#define __STARBARYCENTER__

#include "CSE/Core/CSECore.h"
#include "CSE/Object.h"
#include "CSE/SCStream.h"

_CSE_BEGIN

// Structure of coordinate:
// 
// 0b00000000000000000000000000000000000000000000000000000000000000000000000000000000
//   |---||---||----|||---------||--------------------------------------------------|
//         hrs  min  n sec.expo   sec.mantissa
// 
// hrs:          hour bits (5 - 9 bits)
// min:          minute bits (6 bits)
// n:            sign bit, maybe unused (1 bit)
// sec.expo:     second exponential bits, using IEEE754 double precision (11 bits)
// sec.mantissa: second mantissa bits (52 bits)

struct _Coordinate24_Base80
{
    uint16_t sec3 : 16;
    uint16_t sec2 : 16;
    uint16_t sec1 : 16;
    uint16_t sec0 : 15;
    uint16_t min : 6;
    uint16_t hrs : 5;
};

struct _Coordinate360_Base80
{
    uint16_t sec3 : 16;
    uint16_t sec2 : 16;
    uint16_t sec1 : 16;
    uint16_t sec0 : 15;
    uint16_t min : 6;
    uint16_t hrs : 9;
};

struct _Coordinate90_Base80
{
    uint16_t sec3 : 16;
    uint16_t sec2 : 16;
    uint16_t sec1 : 16;
    uint16_t sec0 : 15;
    uint16_t neg : 1;
    uint16_t min : 6;
    uint16_t hrs : 7;
};

union UCoordinate24;
union Coordinate90;
union UCoordinate360;

union UCoordinate24
{
    _Coordinate24_Base80 _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    UCoordinate24() {};
    UCoordinate24(float64 h);
    UCoordinate24(uint64 h, uint64 m, float64 s);

    uint64 hrs()const;
    uint64 min()const;
    float64 sec()const;

    operator float64();
};

union Coordinate90
{
    _Coordinate90_Base80 _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    Coordinate90() {};
    Coordinate90(float64 d);
    Coordinate90(uint64 d, uint64 m, float64 s);

    uint64 deg()const;
    uint64 min()const;
    float64 sec()const;

    operator float64();
};

union UCoordinate360
{
    _Coordinate360_Base80 _Data;
    uint16_t Bytes[5]; // 80 bits
public:
    UCoordinate360() {};
    UCoordinate360(float64 h);
    UCoordinate360(uint64 h, uint64 m, float64 s);

    uint64 hrs()const;
    uint64 min()const;
    float64 sec()const;

    operator float64();
};

class StarBarycenter
{
public:
    UCoordinate24 RA;
    Coordinate90 Dec;
    float64 Dist;
    _STD shared_ptr<Object> Pointer = nullptr;
};

template<typename _Ty> requires std::is_same_v<_Ty, int>
class _StarBarycen_Base
{
    _Ty __CLR_OR_THIS_CALL _BaseInit()
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

StarBarycenter BarycenterLoader(_STD vector<_CSE _SC table::KeyValue>::iterator& it);
StarBarycenter GetBarycenter(ISCStream _Is, _STD string _Name);

OBarycenterStream& operator<<(OBarycenterStream& _Os, _CSE StarBarycenter _Obj);

_CSE_END

#endif