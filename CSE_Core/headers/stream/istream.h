///////////////////////////////////////////////////////
//                     SC Parser                     //
// ------------------------------------------------- //
// A simple loader for Space Engine sc file          //
///////////////////////////////////////////////////////

#pragma once

#ifndef __CSE_ISTREAM__
#define __CSE_ISTREAM__

#include <istream>
#include <map>
#include <vector>
#include "..\Core\CSECore.h"
#include "..\Core\CelObject.h"

// IStream log level:
// 0 - do not log
// 1 - log errors and warnings only
// 2 - log everything
#define LogLevel    2

// Parse file option
// 0 = Preprocessing only(will affect whole structure!)
// 1 = Parse type only
// 2 = Parse everything
#define PARSE_OPTION 2

#define _ENABLE_ISTREAM_DEBUG false

_CSE_BEGIN

namespace sc 
{ 
    typedef std::map<std::string, std::string> SCMap;
    typedef std::pair<std::string, std::string> KeyValue;
}

class ISCStream
{
#if _ENABLE_ISTREAM_DEBUG == true
public:
#endif
    struct ObjSimpleData
    {
        std::string Type;
        std::string Name;
        std::string Data;

        _SC SCMap BasicData;
        std::string RotationModel = "Simple";
        _SC SCMap RotationIAU;
        std::string PTMode;
        std::string PTDStr;
        int64 LifeCount = 0;
        _SC SCMap Life[2];
        _SC SCMap Interior;
        _SC SCMap Surface;
        _SC SCMap Ocean;
        _SC SCMap OceanComp;
        std::vector<_SC SCMap> Clouds;
        _SC SCMap Atmosphere;
        _SC SCMap AtmComp;
        _SC SCMap Aurora;
        _SC SCMap Rings;
        _SC SCMap AccDisk;
        _SC SCMap Corona;
        _SC SCMap CometTail;
        _SC SCMap Orbit;
    };
private:
    void ClearTemp();

public:
    ISCStream& operator=(const ISCStream& is) = delete;
    std::vector<ObjSimpleData> Catalogs;
    ObjSimpleData Input;
    void PushBack();
    std::vector<std::string> find(std::string Name, size_t* ObjectNumber);
};

_SC_BEGIN
class parser
{
    std::istream& Input;
    std::string Line;
    size_t LineNumber = 1;
public:
    /**
     * SC Parser are constructed from streams.
     */
    parser(std::istream& stream) : Input(stream){}
    parser& operator=(const parser& is) = delete;
    #if PARSE_OPTION == 0
    std::string parse();
    #else
    ISCStream parse();
    #endif

private:
    std::string SkipComments(std::string input);
    std::string Addendl(std::string input);
    void ParseType(std::string::iterator& It, std::string::iterator& End, ISCStream& is);
    KeyValue ParseKeyValue(std::string::iterator& It, const std::string::iterator& End);
    SCMap SaveToMap(const std::string& Info);
};
_SC_END

/**
 * Utility function to parse a file as a SC file. Returns the Object catalog contains all the objects in the file.
 * Throws a parse_exception if the file cannot be opened.
 */
#if PARSE_OPTION == 0
std::string ParseFile(const std::string& FileName);
#else
ISCStream ParseFile(const std::string& FileName);
#endif

Object GetSEObject(ISCStream istream, std::string name);

_CSE_END

#endif