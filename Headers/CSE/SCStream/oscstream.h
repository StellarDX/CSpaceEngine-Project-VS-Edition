////////////////////////////////////////////////////////
//                  SC Output Module                  //
// -------------------------------------------------- //
// Output Celestial object data to .sc file           //
////////////////////////////////////////////////////////

#pragma once

#ifndef __CSE_OSTREAM__
#define __CSE_OSTREAM__

// OStream log level:
// 0 - do not log
// 1 - log errors and warnings only
// 2 - log everything
#define OLogLevel    2

#include "CSE/Core.h"
#include <ostream>
#include <iomanip>
#include <format>
#include <Windows.h>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

extern CSELog Log_OS;

// --Output Base Settings-- //

template<typename _Ty> requires std::is_same_v<_Ty, int>
class _CelObject_Base
{
public:
    // define templatized bitmask/enumerated types, instantiate on demand
    enum _Fmtflags
    {
        // constants for formatting options
        _Fmtmask = 0b11111111111111111111111111111111,
        _Fmtzero = 0b00000000000000000000000000000000
    };

    static constexpr _Fmtflags Physical   = static_cast<_Fmtflags>(0b00000000000000000000000000000001);
    static constexpr _Fmtflags Optical    = static_cast<_Fmtflags>(0b00000000000000000000000000000010);
    static constexpr _Fmtflags Rotation   = static_cast<_Fmtflags>(0b00000000000000000000000000000100);
    static constexpr _Fmtflags Life       = static_cast<_Fmtflags>(0b00000000000000000000000000001000);
    static constexpr _Fmtflags Interior   = static_cast<_Fmtflags>(0b00000000000000000000000000010000);
    static constexpr _Fmtflags Surface    = static_cast<_Fmtflags>(0b00000000000000000000000000100000);
    static constexpr _Fmtflags Ocean      = static_cast<_Fmtflags>(0b00000000000000000000000001000000);
    static constexpr _Fmtflags Clouds     = static_cast<_Fmtflags>(0b00000000000000000000000010000000);
    static constexpr _Fmtflags Atmosphere = static_cast<_Fmtflags>(0b00000000000000000000000100000000);
    static constexpr _Fmtflags Aurora     = static_cast<_Fmtflags>(0b00000000000000000000001000000000);
    static constexpr _Fmtflags Rings      = static_cast<_Fmtflags>(0b00000000000000000000010000000000);
    static constexpr _Fmtflags AccDisk    = static_cast<_Fmtflags>(0b00000000000000000000100000000000);
    static constexpr _Fmtflags Corona     = static_cast<_Fmtflags>(0b00000000000000000001000000000000);
    static constexpr _Fmtflags CometTail  = static_cast<_Fmtflags>(0b00000000000000000010000000000000);
    static constexpr _Fmtflags AutoOrbit  = static_cast<_Fmtflags>(0b00000000000000000100000000000000);
    static constexpr _Fmtflags CustomClim = static_cast<_Fmtflags>(0b00000000000000001000000000000000);

    static constexpr _Fmtflags DisableWM  = static_cast<_Fmtflags>(0b10000000000000000000000000000000);
    static constexpr _Fmtflags Fixed      = static_cast<_Fmtflags>(0b01000000000000000000000000000000);
    static constexpr _Fmtflags NoBooleans = static_cast<_Fmtflags>(0b00100000000000000000000000000000);
    static constexpr _Fmtflags FTidalLock = static_cast<_Fmtflags>(0b00010000000000000000000000000000);
    static constexpr _Fmtflags DisAutoRad = static_cast<_Fmtflags>(0b00001000000000000000000000000000);
    static constexpr _Fmtflags DimAsRadOb = static_cast<_Fmtflags>(0b00000100000000000000000000000000);

    static constexpr _Fmtflags Default    = static_cast<_Fmtflags>(0b00000000000000001111111111111111);

    _Ty __CLR_OR_THIS_CALL _BaseInit()
    {
        return Default; // Default FMTFlag
    }

    bool PecularKey(_STD string Key, uint64* Mode)const noexcept
    {
        if (Key.find("PeriodicTerms") != Key.npos)
        {
            *Mode = 1;
            return 1;
        }
        return 0;
    }
};

_SC_BEGIN

class OutputException : public _STD runtime_error
{
public:
    explicit OutputException(const _STD string& _Message) : runtime_error(_Message) {}
};

template<typename _TyBase, int _DefEncod = CP_UTF8>
class _Smart_Output_Base : public _TyBase
{
    // base class for smart output
public:
    using fmtflags = int;
    using outstate = int; // Preparing, Writing, ...
    using encoding = int; // ANSI, UTF8, ...

    _Smart_Output_Base(const _Smart_Output_Base&) = delete;
    _Smart_Output_Base& operator=(const _Smart_Output_Base&) = delete;

    enum _Outstate
    {
        _Statmask = 0b10111
    };

    static constexpr _Outstate Normal = static_cast<_Outstate>(0b00000); // Normal stste
    static constexpr _Outstate End    = static_cast<_Outstate>(0b00001); // End writing
    static constexpr _Outstate Failed = static_cast<_Outstate>(0b00010); // failed to write
    static constexpr _Outstate Bad    = static_cast<_Outstate>(0b00100); // sth is wrong

    void __CLR_OR_THIS_CALL _SetState(outstate _State, bool _Reraise)
    { // set state, possibly reraise exception
        _State &= _Statmask;
        _State = _State;
        const auto _Filtered = _State & _Except;
        if (_Filtered)
        {
            if (_Reraise){throw;}

            const char* _Msg;
            if (_Filtered & Bad){_Msg = "Something is wrong during writing file.";}
            else if (_Filtered & Failed) {_Msg = "Failed to write file";}
            else {_Msg = "End of file";}

            throw OutputException(_Msg);
        }
    }

    void __CLR_OR_THIS_CALL resetstate(outstate _State = Normal)
    { 
        // set state to argument
        _SetState(_State, false);
    }

    _NODISCARD outstate __CLR_OR_THIS_CALL state()const
    {
        return _State;
    }

    void __CLR_OR_THIS_CALL setstate(outstate _State, bool _Exreraise = false)
    { 
        // merge in state argument, possibly reraise exception
        _SetState(state() | _State, _Exreraise);
    }

private:
    fmtflags _Fmtfl; // format flags
    outstate _State; // stream state
    outstate _Except; // Exceptions
    encoding _Encod; // file encoding
    _STD streamsize _Prec; // field precision, 0 = Auto
    _STD streamsize _KeyWide; // Key wide, 0 = dynamic

protected:
    __CLR_OR_THIS_CALL _Smart_Output_Base(){}
    void __CLR_OR_THIS_CALL _Init()
    {
        _Fmtfl = _TyBase::_BaseInit();
        resetstate();
        _Except = Normal;
        _Encod = _DefEncod;
        _Buf.clear();
        _Prec = 0;
        _KeyWide = 0;
    }

public:
    #define OUTPUT_WATER_MARK(version, date, fmtfl, encod)  \
    (std::string)"// File created by CSpaceEngine\n" +      \
    "// A Open-sourced C++ static library by StellarDX\n" + \
    "// Compiler version : " + version + "\n" +             \
    "// Date :             " + date + "\n" +                \
    "// Format Flags :     " + fmtfl + "\n" +               \
    "// Encoding :         " + encod + "\n"                 \

    table _Buf;

    void clear() { _Buf.clear(); }

    // Setting State

    _NODISCARD bool __CLR_OR_THIS_CALL normal() const 
    {
        return state() == Normal;
    }

    _NODISCARD bool __CLR_OR_THIS_CALL end() const 
    {
        return state() & End;
    }

    _NODISCARD bool __CLR_OR_THIS_CALL fail() const 
    {
        return state() & (Bad | Failed);
    }

    _NODISCARD bool __CLR_OR_THIS_CALL bad() const 
    {
        return state() & Bad;
    }

    _NODISCARD outstate __CLR_OR_THIS_CALL exceptions() const 
    {
        return _Except;
    }

    void __CLR_OR_THIS_CALL exceptions(outstate _Newexcept)
    { 
        // set exception mask to argument
        _Except = _Newexcept & _Statmask;
        setstate(state());
    }

    // FMT Flags

    _NODISCARD fmtflags __CLR_OR_THIS_CALL flags() const
    {
        return _Fmtfl;
    }

    fmtflags __CLR_OR_THIS_CALL flags(fmtflags _Newfmtflags) 
    { 
        // set format flags to argument
        const fmtflags _Oldfmtflags = _Fmtfl;
        _Fmtfl = _Newfmtflags & _TyBase::_Fmtmask;
        return _Oldfmtflags;
    }

    fmtflags __CLR_OR_THIS_CALL setf(fmtflags _Newfmtflags, fmtflags _Mask = _TyBase::_Fmtmask)
    { 
        // merge in format flags argument under mask argument
        const fmtflags _Oldfmtflags = _Fmtfl;
        _Fmtfl = _Oldfmtflags | (_Newfmtflags & _Mask & _TyBase::_Fmtmask);
        return _Oldfmtflags;
    }

    void __CLR_OR_THIS_CALL unsetf(fmtflags _Mask)
    { // clear format flags under mask argument
        _Fmtfl &= ~_Mask;
    }

    // Setting key Width

private:

    _CONSTEXPR20 _STD streamsize _Dynamic_Key_Wide(uint64 Arg)const
    {
        switch (Arg)
        {
        case 0:
            return 5;
            break;
        case 1:
        case 2:
            return 16;
            break;
        case 3:
            return 12;
            break;
        default:
            return 10;
            break;
        }
    }

public:

    _NODISCARD _STD streamsize __CLR_OR_THIS_CALL width() const
    {
        return _KeyWide;
    }

    _STD streamsize __CLR_OR_THIS_CALL width(_STD streamsize _Newwidth) 
    { 
        // set key width to argument
        const _STD streamsize _Oldwidth = _KeyWide;
        _KeyWide = _Newwidth;
        return _Oldwidth;
    }

    // Setting Precision

    _NODISCARD _STD streamsize __CLR_OR_THIS_CALL precision() const
    {
        return _Prec;
    }

    _STD streamsize __CLR_OR_THIS_CALL precision(_STD streamsize _Newprec)
    {
        // set key width to argument
        const _STD streamsize _Oldprec = _Prec;
        _Prec = _Newprec;
        return _Oldprec;
    }

    // Setting Encoding

    _NODISCARD encoding __CLR_OR_THIS_CALL encod() const
    {
        return _Encod;
    }

    encoding __CLR_OR_THIS_CALL encod(encoding _NewEnc)
    {
        // set key width to argument
        const encoding _OldEnc = _Encod;
        _Encod = _NewEnc;
        return _OldEnc;
    }

    // Output

    _STD string _Write(const table& _Buffer, bool DisableWM = false, uint64 WSpace = 0)const
    {   
        using namespace std;
        ostringstream _Os;

        if (!DisableWM && _Fmtfl > 0)
        {
            ostringstream FmtFlHex;
            FmtFlHex << hex << "0x" << _Fmtfl;
            auto DateTime = CSEDateTime::currentDateTime();

            _Os << OUTPUT_WATER_MARK(COMPILER_VERSION, DateTime.date().toString() + ' ' + DateTime.time().toString(), FmtFlHex.str(), to_string(encod())) << '\n';
        }

        bool HasSubTable = false;
        for (size_t i = 0; i < _Buffer.Catalogue.size(); i++)
        {
            if (HasSubTable || (_Buffer.Catalogue[i].SubTable != nullptr && i != 0))
            {
                HasSubTable = false;
                _Os << '\n';
            }
            _STD streamsize _KWide = _KeyWide;
            if (_KeyWide == 0) { _KWide = _Dynamic_Key_Wide(WSpace); }
            else { _KWide = _KeyWide; }
            if (uint64(_KWide) <= _Buffer.Catalogue[i].Key.size()) { _KWide = _Buffer.Catalogue[i].Key.size() + 1; }
            for (uint64 j = 0; j < WSpace; ++j) { _Os << "\t"; }

            // Pecular value list:

            uint64 _PecMode = 0;
            if (_TyBase::PecularKey(_Buffer.Catalogue[i].Key, &_PecMode))
            {
                _Os << _Buffer.Catalogue[i].Key << '\n';
                if (_PecMode == 1)
                {
                    for (uint64 j = 0; j < WSpace; ++j) { _Os << "\t"; }
                    _Os << "{\n";
                }
                _Os << _Buffer.Catalogue[i].Value;
                if (_PecMode == 1)
                {
                    for (uint64 j = 0; j < WSpace; ++j) { _Os << "\t"; }
                    _Os << "}\n";
                }
            }

            // -------------------------------------------------------------------------

            else
            {
                _Os << vformat(("{:" + to_string(_KWide) + "}" + "{}\n").c_str(), make_format_args(_Buffer.Catalogue[i].Key, _Buffer.Catalogue[i].Value));
                if (_Buffer.Catalogue[i].SubTable != nullptr)
                {
                    HasSubTable = true;
                    for (uint64 j = 0; j < WSpace; ++j) { _Os << "\t"; }
                    _Os << "{\n";
                    _Os << _Write(*(_Buffer.Catalogue[i].SubTable), true, WSpace + 1);
                    for (uint64 j = 0; j < WSpace; ++j) { _Os << "\t"; }
                    _Os << "}\n";
                }
            }
        }

        return _Os.str();
    }
};

class object_ostream : public _Smart_Output_Base<_CSE _CelObject_Base<int>>
{
    _STD ostream& fout;
public:
    explicit __CLR_OR_THIS_CALL object_ostream(_STD ostream& _Os) : fout(_Os)
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

    //object_ostream& operator<<(_CSE Object _Obj)
    //{
    //    _Buf.push(MakeTable(*this, _Obj));
    //    return *this;
    //}

    object_ostream& operator<<(object_ostream& (_cdecl* _Manip)(object_ostream&))
    { // insert by calling function with output stream and argument
        (*_Manip)(*this);
        return *this;
    }
};

_SC_END

using OSCStream = _SC object_ostream;

inline OSCStream& __CLRCALL_OR_CDECL DisableAutoOrbit(OSCStream& _Os)
{
    _Os.unsetf(OSCStream::AutoOrbit);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL Fixed(OSCStream& _Os)
{
    _Os.setf(OSCStream::Fixed);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL NoWaterMarks(OSCStream& _Os)
{
    _Os.setf(OSCStream::DisableWM);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL NoBooleans(OSCStream& _Os)
{
    _Os.setf(OSCStream::NoBooleans);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL IgnoreRotationIAU(OSCStream& _Os)
{
    _Os.setf(OSCStream::FTidalLock);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL FlatObjectDimAsRadius(OSCStream& _Os)
{
    _Os.setf(OSCStream::DisAutoRad);
    _Os.setf(OSCStream::DimAsRadOb);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL Simple(OSCStream& _Os)
{
    Log_OS.Out("OSCStream", "WARNING", "[CSE Smart Output] Output mode is switching to simple output mode, this will cause data loss!", OLogLevel, true);
    _Os.flags(0b00100000000000000000000000000000);
    return _Os;
}

inline OSCStream& __CLRCALL_OR_CDECL Resetf(OSCStream& _Os)
{
    _Os.flags(OSCStream::Default);
    return _Os;
}

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif