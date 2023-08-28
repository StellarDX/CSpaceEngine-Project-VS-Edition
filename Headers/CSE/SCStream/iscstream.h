///////////////////////////////////////////////////////
//                     SC Parser                     //
// ------------------------------------------------- //
// A simple loader for Space Engine sc file          //
///////////////////////////////////////////////////////

#pragma once

#ifndef __CSE_ISTREAM__
#define __CSE_ISTREAM__

// IStream log level:
// 0 - do not log
// 1 - log errors and warnings only
// 2 - log everything
#define ILogLevel    2

#include "CSE\Core.h"
#include <memory>
#include <vector>
#include <sstream>
#include <map>

#pragma pack(push, _CRT_PACKING)
#pragma warning(push, _STL_WARNING_LEVEL)
#pragma warning(disable : _STL_DISABLED_WARNINGS)
_STL_DISABLE_CLANG_WARNINGS
#pragma push_macro("new")
#undef new

_CSE_BEGIN

extern CSELog Log_IS;

_SC_BEGIN

using Names = _STD vector<_STD string>;

class ParseException : public _STD runtime_error // output errors
{
public:
    ParseException(const _STD string& Error) : _STD runtime_error{ Error } {}
    ParseException(const _STD string& err, size_t line_number) : _STD runtime_error{ err + " at line " + _STD to_string(line_number) } {}
};

template<typename genType>
concept _SC_TYPES =
(
    vecType<genType>,
    vecIType<genType>,
    vecBType<genType>,
    vecUType<genType>,
    _STD is_same_v<genType, float64>,
    _STD is_same_v<genType, int64>,
    _STD is_same_v<genType, bool>,
    _STD is_same_v<genType, _STD string>,
    _STD is_same_v<genType, Names>
);

template<typename genType>
class Value
{
    genType _Data;
public:
    Value(genType Data) : _Data(Data) {}
    genType get()const
    {
        return _Data;
    }
};

class ValueStr
{
    _STD string _Src;

public:
    ValueStr(_STD string Src) : _Src(Src) {}

    template<typename genType>
    requires (_STD is_same_v<genType, float64> || _STD is_same_v<genType, int64>)
    _STD shared_ptr<Value<genType>> as()
    {
        genType _Dst;
        _STD istringstream is(_Src);
        is >> _Dst;
        return _STD make_shared<Value<genType>>(Value<genType>(_Dst));
    }

    template<typename genType>
    requires (_STD is_same_v<genType, Names>)
    _STD shared_ptr<Value<genType>> as()
    {
        Names _N;
        _STD istringstream _Is(_Src);
        _STD string _Line;
        while (_STD getline(_Is, _Line, '/'))
        {
            _N.push_back(_Line);
        }
        return make_shared<Value<genType>>(Value<genType>(_N));
    }

    template<typename genType>
    requires (_STD is_same_v<genType, _STD string>)
    _STD shared_ptr<Value<genType>> as()
    {
        return make_shared<Value<genType>>(Value<genType>(_Src));
    }

    template<typename genType, typename ElemType> requires 
    (
        vecType<genType>  ||
        vecIType<genType> ||
        vecBType<genType> ||
        vecUType<genType>
    )
    _STD shared_ptr<Value<genType>> as()
    {
        _STD string _VecValues = _Src.substr(1, _Src.size() - 2);
        auto it = _VecValues.begin();
        auto end = _VecValues.end();
        while (it != end && isspace(*it))
        {
            ++it;
        }
        _STD vector<_STD string> Values;
        _STD string _Elm;
        while (it != end)
        {
            if (it != end && (',' == *it || isspace(*it)))
            {
                Values.push_back(_Elm);
                _Elm.clear();
                while (it != end && (*it == ',' || isspace(*it)))
                {
                    ++it;
                }
            }
            if (it != end)
            {
                _Elm += *it;
                ++it;
            }
        }
        Values.push_back(_Elm);
        _STD vector<ElemType> Elems;
        for (size_t i = 0; i < Values.size(); i++)
        {
            ElemType _Dst;
            _STD istringstream is(Values[i]);
            is >> _Dst;
            Elems.push_back(_Dst);
        }
        genType _Vec;
        for (size_t i = 0; i < _Vec.size(); i++)
        {
            _Vec[i] = Elems[i];
        }
        return _STD make_shared<Value<genType>>(Value<genType>(_Vec));
    }

    template<typename genType>
    requires (_STD is_same_v<genType, bool>)
    _STD shared_ptr<Value<genType>> as()
    {
        bool _Val;
        if (_Src == "true") { _Val = true; }
        else { _Val = false; }
        return make_shared<Value<genType>>(Value<genType>(_Val));
    }
};

class table
{
public:
    struct KeyValue
    {
        _STD string Key;
        _STD string Value;
        _STD shared_ptr<table> SubTable;
    };

    _STD vector<KeyValue> Catalogue;

    _NODISCARD _CONSTEXPR20 KeyValue& operator[](uint64 _Pos) noexcept
    {
        return Catalogue[_Pos];
    }

    _CONSTEXPR20 void push(const KeyValue& _KV)
    {
        Catalogue.push_back(_KV);
    }

    _CONSTEXPR20 _STD vector<KeyValue>::iterator find(const _STD string _Key)
    {
        auto it = Catalogue.begin();
        auto end = Catalogue.end();
        while (it != end)
        {
            if (_Key == it->Key){break;}
            ++it;
        }
        return it;
    }

    _CONSTEXPR20 _STD vector<KeyValue>::iterator begin()
    {
        return Catalogue.begin();
    }

    _CONSTEXPR20 _STD vector<KeyValue>::iterator end()
    {
        return Catalogue.end();
    }

    _NODISCARD _STD shared_ptr<ValueStr> GetQualified(const _STD string _Key)
    {
        auto it = find(_Key);
        _CSE_LOG_ERROR(it != Catalogue.end(), Log_IS.Out("ISCStream", "ERROR", "[Table] Key \"" + _Key + "\" is not found in this scope.", ILogLevel, true), nullptr);
        return _STD make_shared<ValueStr>(ValueStr(it->Value));
    }

    template<typename genType>
    _NODISCARD _CONSTEXPR20 _STD map<_STD string, genType> ToMap()const
    {
        _STD map<_STD string, genType> _Final;
        for (size_t i = 0; i < Catalogue.size(); i++)
        {
            genType _Value = ValueStr(Catalogue[i].Value).as<genType>()->get();
            _STD pair<_STD string, genType> _Pair(Catalogue[i].Key, _Value);
            _Final.insert(_Pair);
        }
        return _Final;
    }

    _CONSTEXPR20 void clear() noexcept
    {
        Catalogue.clear();
    }
};

template<typename genType>
requires (_STD is_same_v<genType, float64> || _STD is_same_v<genType, int64>)
genType GetAs(_STD vector<table::KeyValue>::iterator _Iter, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Iter->SubTable->GetQualified(_Key);
    if (nullptr == _Ptr) { return NO_DATA_FLOAT_INF; }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, float64> || _STD is_same_v<genType, int64>)
genType GetAs(_STD shared_ptr<table> _Table, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Table->GetQualified(_Key);
    if (nullptr == _Ptr) { return NO_DATA_FLOAT_INF; }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, _STD string>)
genType GetAs(_STD vector<table::KeyValue>::iterator _Iter, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Iter->SubTable->GetQualified(_Key);
    if (nullptr == _Ptr) { return NO_DATA_STRING; }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, _STD string>)
genType GetAs(_STD shared_ptr<table> _Table, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Table->GetQualified(_Key);
    if (nullptr == _Ptr) { return NO_DATA_STRING; }
    return _Ptr->as<genType>()->get();
}

template<typename genType, typename ElemType> requires
(
    vecType<genType> ||
    vecIType<genType> ||
    vecBType<genType> ||
    vecUType<genType>
)
genType GetAs(_STD vector<table::KeyValue>::iterator _Iter, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Iter->SubTable->GetQualified(_Key);
    if (nullptr == _Ptr) { return genType(NO_DATA_FLOAT_INF); }
    return _Ptr->as<genType, ElemType>()->get();
}

template<typename genType, typename ElemType> requires
(
    vecType<genType> ||
    vecIType<genType> ||
    vecBType<genType> ||
    vecUType<genType>
)
genType GetAs(_STD shared_ptr<table> _Table, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Table->GetQualified(_Key);
    if (nullptr == _Ptr) { return genType(NO_DATA_FLOAT_INF); }
    return _Ptr->as<genType, ElemType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, bool>)
genType GetAs(_STD vector<table::KeyValue>::iterator _Iter, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Iter->SubTable->GetQualified(_Key);
    if (nullptr == _Ptr) { return false; }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, bool>)
genType GetAs(_STD shared_ptr<table> _Table, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Table->GetQualified(_Key);
    if (nullptr == _Ptr) { return false; }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, Names>)
genType GetAs(_STD vector<table::KeyValue>::iterator _Iter, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Iter->SubTable->GetQualified(_Key);
    if (nullptr == _Ptr) { return Names(); }
    return _Ptr->as<genType>()->get();
}

template<typename genType>
requires (_STD is_same_v<genType, Names>)
genType GetAs(_STD shared_ptr<table> _Table, _STD string _Key)
{
    Log_IS.Out("ISCStream", "INFO", "[Table] Loading key \"" + _Key + "\"", ILogLevel, true);
    auto _Ptr = _Table->GetQualified(_Key);
    if (nullptr == _Ptr) { return Names(); }
    return _Ptr->as<genType>()->get();
}

_STD vector<_STD shared_ptr<table>> GetMultipleSubTables(_STD vector<table::KeyValue>::iterator& _Iter, const _STD vector<table::KeyValue>::iterator& _End, const _STD string _Key);

#ifndef _WINNLS_ // enable when winnls.h is missing
//
//  Code Page Default Values.
//  Please Use Unicode, either UTF-16 (as in WCHAR) or UTF-8 (code page CP_ACP)
//
#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation
#endif

class parser
{
    _STD istream& input;
public:
    // --Advenced parsing settings-- //

    // Localization settings
    _STD string LocalInfo = setlocale(LC_ALL, ""); // Set Localization

    // Force convertions of char encoding
    // CP_ACP        - ANSI
    // CP_OEMCP      - OEM
    // CP_MACCP      - MAC
    // CP_THREAD_ACP - current thread's ANSI
    // CP_SYMBOL     - Symbols
    // CP_UTF7       - UTF-7
    // CP_UTF8       - UTF-8
    #define FORCE_CONVERT_CHAR_ENCODING true
    #define SOURCE_ENCODING CP_UTF8 // Char encoding of source file(Default: UTF-8)
    #define TARGET_ENCODING CP_ACP  // Target char encoding to convert(Default: ANSI)

    // ----------------------------- //

    /*
     * SC Parser are constructed from streams.
     */
    parser(_STD istream& is) : input(is){}
    parser& operator=(const parser& is) = delete;

    table parse(UINT CodePage = SOURCE_ENCODING);

private:
    _STD string SkipComments(_STD string input);
    _STD string ParseValue(_STD string::iterator& it, const _STD string::iterator& end);
    _STD shared_ptr<table> ParseSubTable(_STD string::iterator& it, const _STD string::iterator& end);
    _STD string ParseMatrix(_STD string::iterator& it, const _STD string::iterator& end);

    #if FORCE_CONVERT_CHAR_ENCODING
public:
    static _STD string ConvertChar(const char* str, int SrcEncod = SOURCE_ENCODING);
    #endif
};

_SC_END

using ISCStream = _STD shared_ptr<_SC table>;

ISCStream ParseFile(_STD string FileName, UINT CodePage = SOURCE_ENCODING);

_CSE_END

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif