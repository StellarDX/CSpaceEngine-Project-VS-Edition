#include <fstream>
#include <sstream>
#include "..\headers\stream\istream.h"

#define _USE_CSE_DEFINES

#include "..\headers\Core\ConstLists.h"

using namespace std;

_CSE_BEGIN

_SC_BEGIN

_CSE CSELog Log_IS; // Log Module

class ParseException : public runtime_error // output errors
{
public:
	ParseException(const string& Error) : runtime_error{ Error } {}
	ParseException(const string& err, size_t line_number) : runtime_error{ err + " at line " + to_string(line_number) } {}
};

string parser::SkipComments(string input)
{
	string Output;
	size_t Line = 1;

	enum Comments
	{
		NoComment,
		SingleLine,
		MultiLine
	};

	Comments S = NoComment;

	for (size_t i = 0; i < input.size() - 1; i++)
	{
		if ('/' == input[i])
		{
			if ("//" == input.substr(i, 2))
			{
				S = SingleLine;
				continue;
			}
			else if ("/*" == input.substr(i, 2))
			{
				S = MultiLine;
				continue;
			}
		}

		if
		(
			('\n' == input[i] && S == SingleLine) ||
			('*' == input[i] && "*/" == input.substr(i, 2) && S == MultiLine)
		) // End of line
		{
			if (S == MultiLine) { ++i; }
			if (S == SingleLine) { Output.push_back('\n'); }
			S = NoComment;
			continue;
		}

		if (S == NoComment)
		{
			Output.push_back(input[i]);
		}
	}

	return Output;
}

string parser::Addendl(std::string input)
{
	string Output;
	istringstream in(input);
	string Line;
	while (getline(in, Line, '\n'))
	{
		if (Line.empty()){continue;}
		string OutLine = Line;
		int NotSpace = 0;
		int Addsize = 0;
		for (size_t i = 0; i < Line.size(); i++)
		{
			if ('{' == Line[i] && NotSpace != 0)
			{
				OutLine.insert(i + Addsize, "\n");
				++Addsize;

				if (i != Line.size() - 1)
				{
					OutLine.insert(i + Addsize + 1, "\n");
					++Addsize;
				}
			}

			if ('}' == Line[i] && NotSpace != 0)
			{
				OutLine.insert(i + Addsize, "\n");
				++Addsize;
			}

			if (Line[i] != ' ' && Line[i] != '\t') // Consume white space
			{
				++NotSpace;
			}
		}

		Output += (OutLine + '\n');
	}

	return Output;
}

void ConsumeWhiteSpace(string::iterator& It, const string::iterator& End)
{
	while (It != End && (*It == ' ' || *It == '\t'))
	{
		++It;
	}
}

void parser::ParseType(std::string::iterator& It, std::string::iterator& End, ISCStream& is)
{
	enum State
	{
		ParsingType,
		EndType,
		ParsingName,
		EndName
	};

	string Type;
	string Name;

	State S = ParsingType;

	while (It != End)
	{
		switch (S)
		{
		case ParsingType:
			if (*It == ' ' || *It == '\t')
			{
				S = EndType;
				break;
			}
			Type.push_back(*It);
			break;

		case EndType:
			// do nothing...
			if (*It == '\"')
			{
				S = ParsingName;
			}
			break;

		case ParsingName:
			if (*It == '\"')
			{
				S = EndName;
				break;
			}
			Name.push_back(*It);
			break;
		case EndName:
			break;

		default:
			break;
		}

		++It;
	}

	is.Input.Type = Type;
	is.Input.Name = Name;

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Pre-Initialization - " + Name, LogLevel);
	//#endif
}

KeyValue parser::ParseKeyValue(string::iterator& It, const string::iterator& End)
{
	string Key;
	string Value;

	enum State
	{
		KeyStart,
		KeyEnd,
		StringStart,
		StringEnd,
		VecStart,
		VecEnd
	};

	State S = KeyStart;

	while (It != End)
	{
		// Parse Key
		if ((*It == ' ' || *It == '\t') && S != StringStart && S != VecStart)
		{
			S = KeyEnd;
			++It;
			continue;
		}
		if (S == KeyStart)
		{
			Key += *It;
			++It;
			continue;
		}

		// Parse number
		if (S == KeyEnd && isgraph(*It) && (*It != '\"' && *It != '(' && *It != ')') && S != StringStart && S != VecStart)
		{
			Value += *It;
			++It;
			continue;
		}

		// Parse string
		if (*It == '\"' && S == KeyEnd)
		{
			S = StringStart;
			++It;
			continue;
		}
		if (*It == '\"' && S == StringStart)
		{
			S = StringEnd;
			break;
		}
		if (S == StringStart)
		{
			Value += *It;
			++It;
			continue;
		}

		// Parse vector
		if (*It == '(' && S == KeyEnd)
		{
			S = VecStart;
			++It;
			continue;
		}
		if (*It == ')' && S == VecStart)
		{
			S = VecEnd;
			break;
		}
		if (S == VecStart)
		{
			Value += *It;
			++It;
			continue;
		}
	}

	return KeyValue(Key, Value);
}

SCMap parser::SaveToMap(const string& Info)
{
	if (Info.empty())
	{
		return SCMap(); // empty map
	}

	SCMap KeyVal;
	istringstream Inf(Info);
	string Line;
	while (getline(Inf, Line, '\n'))
	{
		auto It = Line.begin();
		auto End = Line.end();
		ConsumeWhiteSpace(It, End);
		if (It == End)
		{
			continue;
		}
		KeyVal.insert(ParseKeyValue(It, End));
	}

	return KeyVal;
}

_SC_END

void ISCStream::ClearTemp()
{
	Input.Type.clear();
	Input.Name.clear();
	Input.Data.clear();

	Input.BasicData.clear();
	Input.Life[0].clear();
	Input.Life[1].clear();
	Input.Surface.clear();
	Input.Ocean.clear();
	Input.Clouds.clear();
	Input.Atmosphere.clear();
	Input.Aurora.clear();
	Input.Rings.clear();
	Input.AccDisk.clear();
	Input.Corona.clear();
	Input.CometTail.clear();
	Input.Orbit.clear();
}

void ISCStream::PushBack()
{
	Catalogs.push_back(Input);
	ClearTemp();
}

vector<string> ParseName(string NameString) // Parse Name
{
	istringstream In(NameString);
	string Temp;
	vector<string> Names;
	while (getline(In, Temp, '/'))
	{
		Names.push_back(Temp);
	}
	return Names;
}

vector<string> ISCStream::find(string Name, size_t* ObjectNumber)
{
	vector<string> Names;
	size_t N = 0;
	bool Found = false;
	while (N < Catalogs.size() && !Found)
	{
		Names = ParseName(Catalogs[N].Name);
		for (int i = 0; i < Names.size(); i++)
		{
			if (Names[i] == Name)
			{
				Found = true;
				*ObjectNumber = N;
			}
		}
		if (!Found)
		{
			++N;
			Names.clear();
		}
	}

	if (!Found) // Throw an error when all the objects are checked but no matching object found
	{
		throw sc::ParseException{ "Object not found in this scope: " + Name };
	}

	return Names; // Return a name list when the object is found.
}

#if PARSE_OPTION == 0
std::string ParseFile(const std::string& FileName)
#else
ISCStream ParseFile(const std::string& FileName)
#endif
{
	ifstream File{ FileName };
	if (!File.is_open())
	{
		throw sc::ParseException{ FileName + " not found" };
	}
	sc::parser SC{ File };

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Loading file \"" + FileName + "\"", LogLevel);
	//#endif
	return SC.parse();
}

_SC_BEGIN
auto Find = [&](string::iterator& It, string& Line, string SubTable) -> bool
{
	return *It == *(SubTable.substr(0, 1).c_str()) && Line.find(SubTable) != Line.npos;
};

#if PARSE_OPTION == 0
std::string parser::parse()
#else
ISCStream parser::parse()
#endif
{
	// Preprocessing
	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Loading - Preprocessing catalog file...", LogLevel);
	//#endif

	#if PARSE_OPTION >= 0
	ostringstream SCString;
	while (getline(Input, Line, '\n'))
	{
		SCString << Line << '\n';
	}
	string SCPreProc = SkipComments(SCString.str());
	SCPreProc = Addendl(SCPreProc);
	#if PARSE_OPTION == 0
	return SCPreProc;
	#endif

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "DONE", LogLevel);
	//#endif

	#endif

	// Parsing type
	#if PARSE_OPTION >= 1

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Loading - Initializing objects phase 1...", LogLevel);
	//#endif

	ISCStream Current;
	istringstream SC(SCPreProc);
	string Line;
	int Layer = 0;
	while (getline(SC, Line, '\n'))
	{
		auto It = Line.begin();
		auto End = Line.end();

		ConsumeWhiteSpace(It, End);

		if (It == End) // Skip enpty lines
		{
			continue;
		}

		if (Line.find("{") != Line.npos)
		{
			if (Layer != 0)
			{
				Current.Input.Data += (Line + '\n');
			}
			++Layer;
			continue;
		}
		if (Line.find("}") != Line.npos)
		{
			--Layer;
			if (Layer != 0)
			{
				Current.Input.Data += (Line + '\n');
			}
			if (Layer == 0)
			{
				Current.PushBack();
			}
			continue;
		}

		if (Layer == 0)
		{
			ParseType(It, End, Current);
		}
		else
		{
			Current.Input.Data += (Line + '\n');
		}
	}

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", to_string(Current.Catalogs.size()) + " Objects loaded", LogLevel);
	//#endif

	#endif

	// Parse Details
	#if PARSE_OPTION >= 2

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Loading - Initializing objects phase 2...", LogLevel);
	//#endif

	enum State
	{
		Base,
		IAUModel,
		InOrbit,
		InLife,
		Interior,
		InSurface,
		InOcean,
		InClouds,
		InAtm,
		InAurora,
		InRings,
		InAccDisk,
		InCorona,
		InCometTail,
	};

	bool PTD = false;
	bool InComp = false;

	for (size_t i = 0; i < Current.Catalogs.size(); i++)
	{
		//#if LogLevel > 0
		_SC Log_IS.Out("ISCStream", "INFO", "Initialization - " + Current.Catalogs[i].Name + "  \t" + "(" + to_string(i + 1) + "/" + to_string(Current.Catalogs.size()) + ")", LogLevel);
		//#endif

		istringstream Source(Current.Catalogs[i].Data);
		string Line;
		string SaveData;

		string IAUStr;
		string OrbitStr;
		int LifeCount = 0;
		string LifeStr[2];
		string InteriorStr;
		string SurfaceStr;
		string OceanStr;
		string OceanCompStr;
		string CloudLayer;
		vector<string> CloudsStr;
		string AtmStr;
		string AtmCompStr;
		string AuroraStr;
		string RingsStr;
		string AccDiskStr;
		string CoronaStr;
		string CometTailStr;

		State S = Base;

		while (getline(Source, Line, '\n'))
		{
			auto It = Line.begin();
			auto End = Line.end();
			sc::ConsumeWhiteSpace(It, End);

			if (It == End) // Skip empty line
			{
				continue;
			}
			if (*It == '{') // Sub table start
			{
				++Layer;
				continue;
			}
			if (*It == '}') // Sub table end
			{
				if (InComp)
				{
					InComp = false;
				}
				if (PTD)
				{
					PTD = false;
				}
				--Layer;
				continue;
			}

			if (Layer == 0)
			{
				if (S == InLife && !LifeStr[LifeCount].empty())
				{
					++LifeCount;
				}

				if (S == InClouds && !CloudLayer.empty())
				{
					CloudsStr.push_back(CloudLayer);
					CloudLayer.clear();
				}

				S = Base;
			}

			// Detect Sub-tables

			if (Find(It, Line, "RotationModel") && S == Base)
			{
				Current.Catalogs[i].RotationModel.clear(); // clear default settings.
				enum State
				{
					NoStr,
					Str
				};
				State T = NoStr;
				while (It != End)
				{
					if (*It == '\"' && T == NoStr)
					{
						T = Str;
						++It;
						continue;
					}
					if (*It == '\"' && T == Str)
					{
						T = NoStr;
						++It;
						continue;
					}
					if (T == Str)
					{
						Current.Catalogs[i].RotationModel += *It;
						++It;
						continue;
					}
					++It;
				}
				S = IAUModel;
				continue;
			}

			if ((Find(It, Line, "PeriodicTermsDiurnal") || Find(It, Line, "PeriodicTermsSecular")) && S == IAUModel)
			{
				if (Find(It, Line, "PeriodicTermsDiurnal"))
				{
					Current.Catalogs[i].PTMode = "Diurnal"; // Day format
				}
				else if (Find(It, Line, "PeriodicTermsSecular"))
				{
					Current.Catalogs[i].PTMode = "Secular"; // Century format
				}
				PTD = true;
				continue;
			}

			if (Find(It, Line, "Orbit") && S == Base)
			{
				S = InOrbit;
				continue;
			}

			if (Find(It, Line, "Life") && S == Base)
			{
				S = InLife;
				continue;
			}

			if (Find(It, Line, "Interior") && S == Base)
			{
				S = Interior;
				continue;
			}

			if (Find(It, Line, "Surface") && S == Base)
			{
				S = InSurface;
				continue;
			}

			if (Find(It, Line, "Ocean") && S == Base)
			{
				S = InOcean;
				continue;
			}

			if (Find(It, Line, "Composition") && (S == InOcean || S == InAtm || S == Interior))
			{
				InComp = true;
				continue;
			}

			if (Find(It, Line, "Clouds") && S == Base)
			{
				S = InClouds;
				continue;
			}

			if (Find(It, Line, "Atmosphere") && S == Base)
			{
				S = InAtm;
				continue;
			}

			if (Find(It, Line, "Aurora") && S == Base)
			{
				S = InAurora;
				continue;
			}

			if (Find(It, Line, "Rings") && S == Base)
			{
				S = InRings;
				continue;
			}

			if (Find(It, Line, "AccretionDisk") && S == Base)
			{
				S = InAccDisk;
				continue;
			}

			if (Find(It, Line, "Corona") && S == Base)
			{
				S = InCorona;
				continue;
			}

			if (Find(It, Line, "CometTail") && S == Base)
			{
				S = InCometTail;
				continue;
			}

			// Transport data in to string
			if (S == IAUModel)
			{
				if (Layer == 1)
				{
					IAUStr += Line + '\n';
				}
				if (Layer == 2 && PTD)
				{
					Current.Catalogs[i].PTDStr += Line + '\n';
				}
				continue;
			}

			if (S == InOrbit)
			{
				OrbitStr += Line + '\n';
				continue;
			}

			if (S == InLife)
			{
				LifeStr[LifeCount] += Line + '\n';
				continue;
			}

			if (S == Interior)
			{
				if (Layer == 1)
				{
					// Do nothing...
				}
				if (Layer == 2 && InComp)
				{
					InteriorStr += Line + '\n';
				}
				continue;
			}

			else if (S == InSurface)
			{
				SurfaceStr += Line + '\n';
				continue;
			}

			else if (S == InOcean)
			{
				if (Layer == 1)
				{
					OceanStr += Line + '\n';
				}
				if (Layer == 2 && InComp)
				{
					OceanCompStr += Line + '\n';
				}
				continue;
			}

			else if (S == InClouds)
			{
				CloudLayer += Line + '\n';
				continue;
			}

			else if (S == InAtm)
			{
				if (Layer == 1)
				{
					AtmStr += Line + '\n';
				}
				if (Layer == 2 && InComp)
				{
					AtmCompStr += Line + '\n';
				}
				continue;
			}

			else if (S == InAurora)
			{
				AuroraStr += Line + '\n';
				continue;
			}

			else if (S == InRings)
			{
				RingsStr += Line + '\n';
				continue;
			}

			else if (S == InAccDisk)
			{
				AccDiskStr += Line + '\n';
				continue;
			}

			else if (S == InCorona)
			{
				CoronaStr += Line + '\n';
				continue;
			}

			else if (S == InCometTail)
			{
				CometTailStr += Line + '\n';
				continue;
			}

			// Parse Basic data
			else
			{
				if (It == End)
				{
					continue;
				}
				Current.Catalogs[i].BasicData.insert(ParseKeyValue(It, End));
			}
		}

		// Parse Sub-table
		Current.Catalogs[i].RotationIAU = SaveToMap(IAUStr);
		Current.Catalogs[i].Orbit = SaveToMap(OrbitStr);
		Current.Catalogs[i].LifeCount = LifeCount;
		Current.Catalogs[i].Life[0] = SaveToMap(LifeStr[0]);
		Current.Catalogs[i].Life[1] = SaveToMap(LifeStr[1]);
		Current.Catalogs[i].Interior = SaveToMap(InteriorStr);
		Current.Catalogs[i].Surface = SaveToMap(SurfaceStr);
		Current.Catalogs[i].Ocean = SaveToMap(OceanStr);
		Current.Catalogs[i].OceanComp = SaveToMap(OceanCompStr);
		for (size_t j = 0; j < CloudsStr.size(); j++)
		{
			Current.Catalogs[i].Clouds.push_back(SaveToMap(CloudsStr[j]));
		}
		Current.Catalogs[i].Atmosphere = SaveToMap(AtmStr);
		Current.Catalogs[i].AtmComp = SaveToMap(AtmCompStr);
		Current.Catalogs[i].Aurora = SaveToMap(AuroraStr);
		Current.Catalogs[i].Rings = SaveToMap(RingsStr);
		Current.Catalogs[i].AccDisk = SaveToMap(AccDiskStr);
		Current.Catalogs[i].Corona = SaveToMap(CoronaStr);
		Current.Catalogs[i].CometTail = SaveToMap(CometTailStr);

		Current.Catalogs[i].Data.clear();
	}

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "DONE", LogLevel);
	//#endif
	#endif

	#if PARSE_OPTION >= 1
	return Current;
	#endif
}
_SC_END

Object GetSEObject(ISCStream istream, string name)
{
	Object Obj;
	size_t ObjectNum = 0;
	Obj.Name = istream.find(name, &ObjectNum);
	Obj.Type = istream.Catalogs[ObjectNum].Type;

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "Loading object - " + Obj.Name[0], LogLevel);
	//#endif

	auto find = [&](_SC SCMap Map, string Key)->bool
	{
		return Map.find(Key) != Map.end();
	};

	auto GetData = [&](_SC SCMap Map, string Key)
	{
		if (find(Map, Key))
		{
			return Map.find(Key)->second;
		}
		else{ return string("None"); }
	};

	auto GetNumber = [&](_SC SCMap Map, string Key)->float64
	{
		if (find(Map, Key))
		{
			return stod(Map.find(Key)->second);
		}
		else { return wrtval(POS_INF_DOUBLE); }
	};

	auto GetBool = [&](_SC SCMap Map, string Key)->bool
	{
		if (find(Map, Key))
		{
			if (Map.find(Key)->second == "true")
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else { return false; }
	};

	auto GetVec3 = [&](_SC SCMap Map, string Key)->vec3
	{
		if (find(Map, Key))
		{
			enum State
			{
				Value,
				NonValue
			};

			string Vec = Map.find(Key)->second;
			int Num = 0;
			string Values[3];
			auto It = Vec.begin();
			auto End = Vec.end();

			State S = Value;

			while (It != End)
			{
				if ((*It == ' ' || *It == '\t' || *It == ',') && S == Value)
				{
					S = NonValue;
					++It;
					continue;
				}
				if (S == NonValue && isgraph(*It))
				{
					S = Value;
					++Num;
					Values[Num] += *It;
					++It;
					continue;
				}
				if (S == Value)
				{
					Values[Num] += *It;
					++It;
					continue;
				}
				++It;
			}

			return vec3(stod(Values[0]), stod(Values[1]), stod(Values[2]));
		}
		else { return vec3(wrtval(POS_INF_DOUBLE)); }
	};

	auto ParseComp = [&](_SC SCMap Map)->map<string, float64>
	{
		map<string, float64> final;
		auto It = Map.begin();
		auto End = Map.end();
		while (It != End)
		{
			final.insert(pair<string, float64>(It->first, stod(It->second)));
			++It;
		}
		return final;
	};

	Obj.ParentBody = GetData(istream.Catalogs[ObjectNum].BasicData, "ParentBody");
	if (Obj.Type == "Star")
	{
		Obj.Class = "Sun";
		Obj.SpecClass = GetData(istream.Catalogs[ObjectNum].BasicData, "Class");
	}
	else
	{
		Obj.Class = GetData(istream.Catalogs[ObjectNum].BasicData, "Class");
	}
	// Masses
	if (find(istream.Catalogs[ObjectNum].BasicData, "Mass"))
	{
		Obj.Mass = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Mass") * MassEarth;
	}
	else if (find(istream.Catalogs[ObjectNum].BasicData, "MassSol"))
	{
		Obj.Mass = GetNumber(istream.Catalogs[ObjectNum].BasicData, "MassSol") * MassSol;
	}
	else if (find(istream.Catalogs[ObjectNum].BasicData, "MassKg"))
	{
		Obj.Mass = GetNumber(istream.Catalogs[ObjectNum].BasicData, "MassKg");
	}
	// Radiuses(Convert to Dimensions)
	if (find(istream.Catalogs[ObjectNum].BasicData, "Dimensions"))
	{
		Obj.Dimensions = GetVec3(istream.Catalogs[ObjectNum].BasicData, "Dimensions") * 1000.0;
	}
	else if (find(istream.Catalogs[ObjectNum].BasicData, "DimensionsSol"))
	{
		Obj.Dimensions = GetVec3(istream.Catalogs[ObjectNum].BasicData, "DimensionsSol") * (float64)RadSol;
	}
	if (find(istream.Catalogs[ObjectNum].BasicData, "Radius") || find(istream.Catalogs[ObjectNum].BasicData, "RadSol"))
	{
		if (find(istream.Catalogs[ObjectNum].BasicData, "Oblateness"))
		{
			vec3 flattening;
			float64 EqRadius;
			if (istream.Catalogs[ObjectNum].BasicData.find("Oblateness")->second.find(' ') != istream.Catalogs[ObjectNum].BasicData.find("Oblateness")->second.npos
			 || istream.Catalogs[ObjectNum].BasicData.find("Oblateness")->second.find(',') != istream.Catalogs[ObjectNum].BasicData.find("Oblateness")->second.npos)
			{
				flattening = GetVec3(istream.Catalogs[ObjectNum].BasicData, "Oblateness");
			}
			else
			{
				flattening = vec3(0.0, GetNumber(istream.Catalogs[ObjectNum].BasicData, "Oblateness"), 0.0);
			}

			if (find(istream.Catalogs[ObjectNum].BasicData, "Radius"))
			{
				EqRadius = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Radius") * 1000;
			}
			else if (find(istream.Catalogs[ObjectNum].BasicData, "RadSol"))
			{
				EqRadius = GetNumber(istream.Catalogs[ObjectNum].BasicData, "RadSol") * RadSol;
			}

			if (!isinf(EqRadius) && !isinf(flattening.x) && !isinf(flattening.y) && !isinf(flattening.z))
			{
				Obj.Dimensions = EqRadius - (EqRadius * flattening) * 2.0;
			}
		}
		else
		{
			if (find(istream.Catalogs[ObjectNum].BasicData, "Radius"))
			{
				Obj.Dimensions = vec3(GetNumber(istream.Catalogs[ObjectNum].BasicData, "Radius") * 1000 * 2.0);
			}
			else if (find(istream.Catalogs[ObjectNum].BasicData, "RadSol"))
			{
				Obj.Dimensions = vec3(GetNumber(istream.Catalogs[ObjectNum].BasicData, "RadSol") * RadSol * 2.0);
			}
		}
	}
	Obj.InertiaMoment = GetNumber(istream.Catalogs[ObjectNum].BasicData, "InertiaMoment");
	if (find(istream.Catalogs[ObjectNum].BasicData, "Albedo"))
	{
		Obj.AlbedoBond = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Albedo");
		Obj.AlbedoGeom = Obj.AlbedoBond;
	}
	Obj.AlbedoBond = GetNumber(istream.Catalogs[ObjectNum].BasicData, "AlbedoBond");
	Obj.AlbedoGeom = GetNumber(istream.Catalogs[ObjectNum].BasicData, "AlbedoGeom");

	Obj.Teff = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Teff");
	if (find(istream.Catalogs[ObjectNum].BasicData, "Luminosity"))
	{
		Obj.Luminosity = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Luminosity") * SolarLum;
	}
	else if (find(istream.Catalogs[ObjectNum].BasicData, "Lum")) // an alternative key for luminosity
	{
		Obj.Luminosity = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Lum") * SolarLum;
	}
	Obj.LumBol = GetNumber(istream.Catalogs[ObjectNum].BasicData, "LumBol") * SolarLum;
	Obj.FeH = GetNumber(istream.Catalogs[ObjectNum].BasicData, "FeH");
	Obj.Age = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Age") * 1000000000;

	Obj.Brightness = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Brightness");
	Obj.BrightnessReal = GetNumber(istream.Catalogs[ObjectNum].BasicData, "BrightnessReal");

	Obj.Color = GetVec3(istream.Catalogs[ObjectNum].BasicData, "Color");

	Obj.AbsMagn = GetNumber(istream.Catalogs[ObjectNum].BasicData, "AbsMagn");
	Obj.SlopeParam = GetNumber(istream.Catalogs[ObjectNum].BasicData, "SlopeParam");

	Obj.KerrSpin = GetNumber(istream.Catalogs[ObjectNum].BasicData, "KerrSpin");
	Obj.KerrCharge = GetNumber(istream.Catalogs[ObjectNum].BasicData, "KerrCharge");

	// Rotation

	#if OBJ_DEFAULT_ROTATION_MODEL <= 1
	Obj.Rotation.RotationPeriod = GetNumber(istream.Catalogs[ObjectNum].BasicData, "RotationPeriod") * 3600;
	Obj.Rotation.RotationEpoch = GetNumber(istream.Catalogs[ObjectNum].BasicData, "RotationEpoch");
	Obj.Rotation.RotationOffset = GetNumber(istream.Catalogs[ObjectNum].BasicData, "RotationOffset");
	Obj.Rotation.Obliquity = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Obliquity");
	Obj.Rotation.EqAscendNode = GetNumber(istream.Catalogs[ObjectNum].BasicData, "EqAscendNode");
	Obj.Rotation.Precession = GetNumber(istream.Catalogs[ObjectNum].BasicData, "Precession");
	#if OBJ_DEFAULT_ROTATION_MODEL == 1
	Obj.RotationModel = istream.Catalogs[ObjectNum].RotationModel;
	Obj.IAU.Epoch = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "Epoch");
	Obj.IAU.PoleRA = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "PoleRA");
	Obj.IAU.PoleRARate = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "PoleRARate");
	Obj.IAU.PoleDec = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "PoleDec");
	Obj.IAU.PoleDecRate = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "PoleDecRate");
	Obj.IAU.PrimeMeridian = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "PrimeMeridian");
	Obj.IAU.RotationRate = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "RotationRate");
	Obj.IAU.RotationAccel = GetNumber(istream.Catalogs[ObjectNum].RotationIAU, "RotationAccel");
	#endif
	#elif OBJ_DEFAULT_ROTATION_MODEL == 2
	Obj.RotationModel = "Moon-DE";
	//Obj.RotationModel = istream.Catalogs[ObjectNum].RotationModel;
	#endif
	Obj.TidalLocked = GetBool(istream.Catalogs[ObjectNum].BasicData, "TidalLocked");

	// Orbit
	#if OBJ_DEFAULT_STATIC_FIXED_POS == 0
	Obj.StaticPos = GetVec3(istream.Catalogs[ObjectNum].BasicData, "StaticPosXYZ");
	Obj.FixedPos = GetVec3(istream.Catalogs[ObjectNum].BasicData, "FixedPosXYZ");
	#elif OBJ_DEFAULT_STATIC_FIXED_POS == 1
	Obj.StaticPos = GetVec3(istream.Catalogs[ObjectNum].BasicData, "StaticPosPolar");
	Obj.FixedPos = GetVec3(istream.Catalogs[ObjectNum].BasicData, "FixedPosPolar");
	#endif
	Obj.Orbit.AnalyticModel = GetData(istream.Catalogs[ObjectNum].Orbit, "AnalyticModel");
	Obj.Orbit.RefPlane = GetData(istream.Catalogs[ObjectNum].Orbit, "RefPlane");
	Obj.Orbit.Eccentricity = GetNumber(istream.Catalogs[ObjectNum].Orbit, "Eccentricity");
	// Semi-major axis(convert to Pericenter distance)
	if ((!isinf(Obj.Orbit.Eccentricity) && Obj.Orbit.Eccentricity < 1) || isinf(Obj.Orbit.Eccentricity)) // Circular or ecliptical orbit
	{
		if (find(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxis") || find(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxisKm"))
		{
			float64 SemiMajorAxis;
			if (find(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxis"))
			{
				SemiMajorAxis = GetNumber(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxis") * AU;
			}
			else if (find(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxisKm"))
			{
				SemiMajorAxis = GetNumber(istream.Catalogs[ObjectNum].Orbit, "SemiMajorAxisKm") * 1000;
			}
			Obj.Orbit.PericenterDist = SemiMajorAxis - SemiMajorAxis * Obj.Orbit.Eccentricity;
		}
		else if (find(istream.Catalogs[ObjectNum].Orbit, "PericenterDist"))
		{
			Obj.Orbit.PericenterDist = GetNumber(istream.Catalogs[ObjectNum].Orbit, "PericenterDist");
		}
	}
	else if (!isinf(Obj.Orbit.Eccentricity) && Obj.Orbit.Eccentricity >= 1) // Parabolic or hyperbolic orbit
	{
		Obj.Orbit.PericenterDist = GetNumber(istream.Catalogs[ObjectNum].Orbit, "PericenterDist");
	}
	// Periods
	if (find(istream.Catalogs[ObjectNum].Orbit, "Period"))
	{
		Obj.Orbit.Period = GetNumber(istream.Catalogs[ObjectNum].Orbit, "Period") * TropicalYear;
	}
	else if (find(istream.Catalogs[ObjectNum].Orbit, "PeriodDays"))
	{
		Obj.Orbit.Period = GetNumber(istream.Catalogs[ObjectNum].Orbit, "PeriodDays") * SiderealDay; // Synodic days???
	}
	Obj.Orbit.GravParam = GetNumber(istream.Catalogs[ObjectNum].Orbit, "GravParam");
	Obj.Orbit.Inclination = GetNumber(istream.Catalogs[ObjectNum].Orbit, "Inclination");
	Obj.Orbit.AscendingNode = GetNumber(istream.Catalogs[ObjectNum].Orbit, "AscendingNode");
	Obj.Orbit.Epoch = GetNumber(istream.Catalogs[ObjectNum].Orbit, "Epoch");
	// Argument of perihelion
	if (find(istream.Catalogs[ObjectNum].Orbit, "ArgOfPericenter"))
	{
		Obj.Orbit.ArgOfPericenter = GetNumber(istream.Catalogs[ObjectNum].Orbit, "ArgOfPericenter");
	}
	else if (find(istream.Catalogs[ObjectNum].Orbit, "ArgOfPericen"))
	{
		Obj.Orbit.ArgOfPericenter = GetNumber(istream.Catalogs[ObjectNum].Orbit, "ArgOfPericen");
	}
	else if (find(istream.Catalogs[ObjectNum].Orbit, "LongOfPericen"))
	{
		float64 LongOfPericen = GetNumber(istream.Catalogs[ObjectNum].Orbit, "LongOfPericen");
		Obj.Orbit.ArgOfPericenter = LongOfPericen - Obj.Orbit.AscendingNode;
	}
	// Mean Anomaly
	if (find(istream.Catalogs[ObjectNum].Orbit, "MeanAnomaly"))
	{
		Obj.Orbit.MeanAnomaly = GetNumber(istream.Catalogs[ObjectNum].Orbit, "MeanAnomaly");
	}
	else if (find(istream.Catalogs[ObjectNum].Orbit, "MeanLongitude"))
	{
		float64 MeanLongitude = GetNumber(istream.Catalogs[ObjectNum].Orbit, "MeanLongitude");
		Obj.Orbit.MeanAnomaly = MeanLongitude - Obj.Orbit.AscendingNode - Obj.Orbit.ArgOfPericenter;
	}

	// Life
	Obj.NoLife = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoLife") || istream.Catalogs[ObjectNum].LifeCount == 0;
	if (!Obj.NoLife)
	{
		Obj.LifeCount = istream.Catalogs[ObjectNum].LifeCount;
		for (int64 i = 0; i < Obj.LifeCount; i++)
		{
			Obj.Life[i].Class = GetData(istream.Catalogs[ObjectNum].Life[i], "Class");
			Obj.Life[i].Type = GetData(istream.Catalogs[ObjectNum].Life[i], "Type");
			Obj.Life[i].Biome = ParseName(GetData(istream.Catalogs[ObjectNum].Life[i], "Biome"));
			Obj.Life[i].Panspermia = GetBool(istream.Catalogs[ObjectNum].Life[i], "Panspermia");
		}
	}

	// Interior
	Obj.Interior = ParseComp(istream.Catalogs[ObjectNum].Interior);

	// Surface
	if (!istream.Catalogs[ObjectNum].Surface.empty()) { Obj.EnableSurface = true; }
	if (Obj.EnableSurface)
	{
		Obj.Surface.Preset = GetData(istream.Catalogs[ObjectNum].Surface, "Preset");
		Obj.Surface.SurfStyle = GetNumber(istream.Catalogs[ObjectNum].Surface, "SurfStyle");
		Obj.Surface.Randomize = GetVec3(istream.Catalogs[ObjectNum].Surface, "Randomize");
		Obj.Surface.colorParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "colorDistMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "colorDistFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "icecapLatitude"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "tropicLatitude")
		);
		Obj.Surface.climateParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climatePole"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateTropic"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateEquator"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "tropicWidth")
		);
		Obj.Surface.plantsParams1 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateGrassMin"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateGrassMax"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateForestMin"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateForestMax")
		);
		Obj.Surface.plantsParams2 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateSteppeMin"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "climateSteppeMax"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "humidity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "plantsBiomeOffset") // Unused
		);
		Obj.Surface.mainParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "mainFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "terraceProb"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "oceanType"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].Surface, "TidalLocked") // Unused
		);
		Obj.Surface.textureParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "detailScale"), // ?
			0.0,
			GetNumber(istream.Catalogs[ObjectNum].Surface, "venusMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "venusFreq")
		);
		Obj.Surface.mareParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "seaLevel"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "mareFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "mareDensity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "icecapHeight")
		);
		Obj.Surface.montesParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "montesMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "montesFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "montesFraction"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "montesSpiky")
		);
		Obj.Surface.dunesParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "dunesMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "dunesFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "dunesFraction"), // ?
			GetNumber(istream.Catalogs[ObjectNum].Surface, "drivenDarkening")
		);
		Obj.Surface.hillsParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "hillsMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "hillsFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "hillsFraction"), // ?
			GetNumber(istream.Catalogs[ObjectNum].Surface, "hills2Fraction") // ?
		);
		Obj.Surface.riversParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riversMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riversFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riversSin"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "beachWidth")
		);
		Obj.Surface.riftsParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riftsMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riftsFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "riftsSin"),
			0.0
		);
		Obj.Surface.canyonsParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "canyonsMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "canyonsFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "canyonsFraction"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "erosion")
		);
		Obj.Surface.cracksParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cracksMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cracksFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cracksOctaves"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "craterRayedFactor")
		);
		Obj.Surface.craterParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "craterMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "craterFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "craterDensity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "craterOctaves")
		);
		Obj.Surface.volcanoParams1 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoDensity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoOctaves")
		);
		Obj.Surface.volcanoParams2 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoActivity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoFlows"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoRadius"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "volcanoTemp")
		);
		Obj.Surface.lavaParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "lavaCoverage"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].Surface, "snowLevel"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "surfTemperature"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].Surface, "heightTempGrad")
		);
		Obj.Surface.lavaDetails = vec3
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "lavaCoverTidal"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "lavaCoverSun"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "lavaCoverYoung")
		);
		Obj.Surface.cycloneParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneMagn"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneDensity"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneOctaves")
		);
		Obj.Surface.cycloneParams2 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneMagn2"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneFreq2"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneLatitude2"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "cycloneOctaves2")
		);

		Obj.Surface.DiffMap = GetData(istream.Catalogs[ObjectNum].Surface, "DiffMap");
		Obj.Surface.DiffMapAlpha = GetData(istream.Catalogs[ObjectNum].Surface, "DiffMapAlpha");

		Obj.Surface.BumpMap = GetData(istream.Catalogs[ObjectNum].Surface, "BumpMap");
		Obj.Surface.BumpHeight = GetNumber(istream.Catalogs[ObjectNum].Surface, "BumpHeight");
		Obj.Surface.BumpOffset = GetNumber(istream.Catalogs[ObjectNum].Surface, "BumpOffset");

		Obj.Surface.GlowMap = GetData(istream.Catalogs[ObjectNum].Surface, "GlowMap");
		Obj.Surface.GlowMode = GetData(istream.Catalogs[ObjectNum].Surface, "GlowMode");
		Obj.Surface.GlowColor = GetVec3(istream.Catalogs[ObjectNum].Surface, "GlowColor");
		Obj.Surface.GlowBright = GetNumber(istream.Catalogs[ObjectNum].Surface, "GlowBright");

		Obj.Surface.SpecMap = GetData(istream.Catalogs[ObjectNum].Surface, "SpecMap");
		Obj.Surface.RoughnessBias = GetNumber(istream.Catalogs[ObjectNum].Surface, "RoughnessBias");
		Obj.Surface.RoughnessScale = GetNumber(istream.Catalogs[ObjectNum].Surface, "RoughnessScale");
		Obj.Surface.SpecParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpecBrightIce"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpecPowerWater"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpecPowerIce"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpecPowerWater")
		);
		Obj.Surface.SpecularScale = GetNumber(istream.Catalogs[ObjectNum].Surface, "SpecularScale");

		Obj.Surface.Hapke = GetNumber(istream.Catalogs[ObjectNum].Surface, "Hapke");
		Obj.Surface.HapkeParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpotBright"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpotWidth"),
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpotBrightCB"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].Surface, "SpotWidthCB") // Unused
		);
		Obj.Surface.DayAmbient = GetNumber(istream.Catalogs[ObjectNum].Surface, "DayAmbient");
		Obj.Surface.ModulateColor = GetVec3(istream.Catalogs[ObjectNum].Surface, "ModulateColor");
	}

	// Ocean
	Obj.NoOcean = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoOcean");
	if (!Obj.NoOcean)
	{
		if (find(istream.Catalogs[ObjectNum].Ocean, "Height"))
		{
			Obj.Ocean.Height = GetNumber(istream.Catalogs[ObjectNum].Ocean, "Height");
		}
		else if (find(istream.Catalogs[ObjectNum].Ocean, "Depth"))
		{
			Obj.Ocean.Height = GetNumber(istream.Catalogs[ObjectNum].Ocean, "Depth");
		}

		Obj.Ocean.Hapke = GetNumber(istream.Catalogs[ObjectNum].Ocean, "Hapke");
		Obj.Ocean.HapkeParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Ocean, "SpotBright"),
			GetNumber(istream.Catalogs[ObjectNum].Ocean, "SpotWidth"),
			GetNumber(istream.Catalogs[ObjectNum].Ocean, "SpotBrightCB"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].Ocean, "SpotWidthCB") // Unused
		);
		Obj.Ocean.DayAmbient = GetNumber(istream.Catalogs[ObjectNum].Ocean, "DayAmbient");
		Obj.Ocean.ModulateBright = GetNumber(istream.Catalogs[ObjectNum].Ocean, "ModulateBright");

		Obj.Ocean.Composition = ParseComp(istream.Catalogs[ObjectNum].OceanComp);
	}

	// Clouds
	Obj.NoClouds = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoClouds");
	if (!istream.Catalogs[ObjectNum].Clouds.empty() && !Obj.NoClouds)
	{
		Obj.Clouds.cloudsParams1 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "mainFreq"),
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "mainOctaves"),
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "stripeZones"),
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "stripeTwist")
		);
		Obj.Clouds.cloudsParams2 = vec4
		(
			0.0,
			(float64)istream.Catalogs[ObjectNum].Clouds.size(),
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "stripeFluct"),
			GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "Coverage")
		);
		Obj.Clouds.RingsWinter = GetNumber(istream.Catalogs[ObjectNum].Clouds[0], "RingsWinter");
		for (size_t i = 0; i < Obj.Clouds.cloudsParams2.y; i++)
		{
			CloudsParam::LayerDetails Layer;
			Layer.DiffMap = GetData(istream.Catalogs[ObjectNum].Clouds[i], "DiffMap");
			Layer.BumpMap = GetData(istream.Catalogs[ObjectNum].Clouds[i], "BumpMap");
			Layer.Height = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "Height");
			Layer.Velocity = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "Velocity");
			Layer.BumpHeight = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "BumpHeight");
			Layer.BumpOffset = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "BumpOffset");
			Layer.Hapke = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "Hapke");
			Layer.HapkeParams = vec4
			(
				GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "SpotBright"),
				GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "SpotWidth"),
				GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "SpotBrightCB"), // Unused
				GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "SpotWidthCB") // Unused
			);
			Layer.DayAmbient = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "DayAmbient");
			Layer.ModulateColor = vec4
			(
				GetVec3(istream.Catalogs[ObjectNum].Clouds[i], "ModulateColor"),
				GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "Opacity")
			);
			Layer.ModulateBright = GetNumber(istream.Catalogs[ObjectNum].Clouds[i], "ModulateBright");
			Obj.Clouds.Layer.push_back(Layer);
		}
	}

	// Atmosphere
	Obj.NoAtmosphere = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoAtmosphere");
	if (!Obj.NoAtmosphere)
	{
		Obj.Atmosphere.Model = GetData(istream.Catalogs[ObjectNum].Atmosphere, "Model");
		Obj.Atmosphere.Height = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Height");
		Obj.Atmosphere.Density = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Density");
		Obj.Atmosphere.Pressure = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Pressure");
		Obj.Atmosphere.Greenhouse = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Greenhouse");
		Obj.Atmosphere.Bright = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Bright");
		Obj.Atmosphere.Opacity = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Opacity");
		Obj.Atmosphere.SkyLight = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "SkyLight");
		Obj.Atmosphere.Hue = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Hue");
		Obj.Atmosphere.Saturation = GetNumber(istream.Catalogs[ObjectNum].Atmosphere, "Saturation");
		Obj.Atmosphere.Composition = ParseComp(istream.Catalogs[ObjectNum].AtmComp);
	}

	// Aurora
	Obj.NoAurora = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoAurora");
	if (!Obj.NoAurora)
	{
		Obj.Aurora.Height = GetNumber(istream.Catalogs[ObjectNum].Aurora, "Height");
		Obj.Aurora.North.Lat = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthLat");
		Obj.Aurora.North.Lon = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthLon");
		Obj.Aurora.North.Radius = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthRadius");
		Obj.Aurora.North.Width = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthWidth");
		Obj.Aurora.North.Rings = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthRings");
		Obj.Aurora.North.Bright = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthBright");
		Obj.Aurora.North.FlashFreq = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthFlashFreq");
		Obj.Aurora.North.MoveSpeed = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthMoveSpeed");
		Obj.Aurora.North.Particles = GetNumber(istream.Catalogs[ObjectNum].Aurora, "NorthParticles");
		Obj.Aurora.South.Lat = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthLat");
		Obj.Aurora.South.Lon = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthLon");
		Obj.Aurora.South.Radius = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthRadius");
		Obj.Aurora.South.Width = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthWidth");
		Obj.Aurora.South.Rings = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthRings");
		Obj.Aurora.South.Bright = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthBright");
		Obj.Aurora.South.FlashFreq = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthFlashFreq");
		Obj.Aurora.South.MoveSpeed = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthMoveSpeed");
		Obj.Aurora.South.Particles = GetNumber(istream.Catalogs[ObjectNum].Aurora, "SouthParticles");
		Obj.Aurora.TopColor = GetVec3(istream.Catalogs[ObjectNum].Aurora, "TopColor");
		Obj.Aurora.BottomColor = GetVec3(istream.Catalogs[ObjectNum].Aurora, "BottomColor");
	}

	// Rings
	Obj.NoRings = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoRings");
	if (!Obj.NoRings)
	{
		Obj.Rings.Texture = GetData(istream.Catalogs[ObjectNum].Rings, "Texture");
		Obj.Rings.Radiuses = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "InnerRadius"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "OuterRadius"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "EdgeRadius"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "MeanRadius")
		);
		Obj.Rings.Thickness = GetNumber(istream.Catalogs[ObjectNum].Rings, "Thickness");
		Obj.Rings.RockParam = vec3
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "RocksMaxSize"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "RocksSpacing"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "DustDrawDist")
		);
		Obj.Rings.ChartRadius = GetNumber(istream.Catalogs[ObjectNum].Rings, "ChartRadius");
		Obj.Rings.RotationPeriod = GetNumber(istream.Catalogs[ObjectNum].Rings, "RotationPeriod");
		Obj.Rings.Brightness = vec3
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "Brightness"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "FrontBright"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "BackBright")
		);
		Obj.Rings.ShadowParam = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "Density"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "Opacity"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "SelfShadow"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "PlanetShadow")
		);
		Obj.Rings.Hapke = GetNumber(istream.Catalogs[ObjectNum].Rings, "Hapke");
		Obj.Rings.HapkeParams = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "SpotBright"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "SpotWidth"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "SpotBrightCB"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "SpotWidthCB")
		);
		Obj.Rings.Shape = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Rings, "frequency"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "densityScale"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "densityOffset"),
			GetNumber(istream.Catalogs[ObjectNum].Rings, "densityPower")
		);
		Obj.Rings.colorContrast = GetNumber(istream.Catalogs[ObjectNum].Rings, "colorContrast");
		Obj.Rings.FrontColor = GetVec3(istream.Catalogs[ObjectNum].Rings, "FrontColor");
		Obj.Rings.BackThickColor = GetVec3(istream.Catalogs[ObjectNum].Rings, "BackThickColor");
		Obj.Rings.BackIceColor = GetVec3(istream.Catalogs[ObjectNum].Rings, "BackIceColor");
		Obj.Rings.BackDustColor = GetVec3(istream.Catalogs[ObjectNum].Rings, "BackDustColor");
	}

	// Accretion Disk
	Obj.NoAccretionDisk = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoAccretionDisk");
	if (!Obj.NoAccretionDisk)
	{
		float64 InnerRadius;
		if (find(istream.Catalogs[ObjectNum].AccDisk, "InnerRadiusKm"))
		{
			InnerRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "InnerRadiusKm") * 1000;
		}
		else
		{
			InnerRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "InnerRadius") * AU;
		}

		float64 OuterRadius;
		if (find(istream.Catalogs[ObjectNum].AccDisk, "OuterRadiusKm"))
		{
			OuterRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "OuterRadiusKm") * 1000;
		}
		else
		{
			OuterRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "OuterRadius") * AU;
		}

		float64 InnerThickness;
		if (find(istream.Catalogs[ObjectNum].AccDisk, "InnerThicknessKm"))
		{
			InnerThickness = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "InnerThicknessKm") * 1000;
		}
		else
		{
			InnerThickness = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "InnerThickness") * AU;
		}

		float64 OuterThickness;
		if (find(istream.Catalogs[ObjectNum].AccDisk, "OuterThicknessKm"))
		{
			OuterThickness = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "OuterThicknessKm") * 1000;
		}
		else
		{
			OuterThickness = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "OuterThickness") * AU;
		}

		Obj.AccretionDisk.Radiuses = vec4
		(
			InnerRadius,
			OuterRadius,
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "BlackHoleRg"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "GravLensScale") // Unused
		);

		if (find(istream.Catalogs[ObjectNum].AccDisk, "JetRadiusKm"))
		{
			Obj.AccretionDisk.JetRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "JetRadiusKm") * 1000;
		}
		else
		{
			Obj.AccretionDisk.JetRadius = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "JetRadius") * AU;
		}

		Obj.AccretionDisk.JetLength = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "JetLength");
		Obj.AccretionDisk.DiskParams1 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "Temperature"),
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "TwistMagn"),
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "starTime"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "diskTime") // Unused
		);
		Obj.AccretionDisk.DiskParams2 = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "tempShift"), // Unused
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "Brightness"),
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "Opacity"),
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "starRotAng") // Unused
		);
		Obj.AccretionDisk.DiskParams3 = vec4
		(
			InnerThickness,
			OuterThickness,
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "DetailScaleR"),
			GetNumber(istream.Catalogs[ObjectNum].AccDisk, "DetailScaleV") // Unused
		);
		Obj.AccretionDisk.ThicknessPow = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "ThicknessPow");
		Obj.AccretionDisk.AccretionRate = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "AccretionRate");
		Obj.AccretionDisk.Density = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "Density");
		Obj.AccretionDisk.Luminosity = GetNumber(istream.Catalogs[ObjectNum].AccDisk, "Luminosity");
	}

	// Star Corona
	Obj.NoCorona = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoCorona") || Obj.Type != "Star";
	if (!Obj.NoCorona)
	{
		Obj.Corona.Shape = vec4
		(
			GetNumber(istream.Catalogs[ObjectNum].Corona, "RayDensity"),
			GetNumber(istream.Catalogs[ObjectNum].Corona, "RayCurv"),
			GetNumber(istream.Catalogs[ObjectNum].Corona, "Radius"),
			GetNumber(istream.Catalogs[ObjectNum].Corona, "SinFreq") // Unused
		);
		Obj.Corona.Period = GetNumber(istream.Catalogs[ObjectNum].Corona, "Period");
		Obj.Corona.Bright = GetNumber(istream.Catalogs[ObjectNum].Corona, "Brightness");
	}

	// Comet Tail
	Obj.NoCometTail = GetBool(istream.Catalogs[ObjectNum].BasicData, "NoCometTail");
	if (!Obj.NoCometTail)
	{
		Obj.CometTail.MaxLength = GetNumber(istream.Catalogs[ObjectNum].CometTail, "MaxLength");
		Obj.CometTail.GasToDust = GetNumber(istream.Catalogs[ObjectNum].CometTail, "GasToDust");
		Obj.CometTail.Particles = GetNumber(istream.Catalogs[ObjectNum].CometTail, "Particles");
		Obj.CometTail.GasBright = GetNumber(istream.Catalogs[ObjectNum].CometTail, "GasBright");
		Obj.CometTail.DustBright = GetNumber(istream.Catalogs[ObjectNum].CometTail, "DustBright");
		Obj.CometTail.GasColor = GetVec3(istream.Catalogs[ObjectNum].CometTail, "GasColor");
		Obj.CometTail.DustColor = GetVec3(istream.Catalogs[ObjectNum].CometTail, "DustColor");
	}

	//#if LogLevel > 0
	_SC Log_IS.Out("ISCStream", "INFO", "DONE", LogLevel);
	//#endif

	return Obj;
}

_CSE_END
