#include "CSE/SCStream.h"
#include <fstream>
#include <sstream>

#if FORCE_CONVERT_CHAR_ENCODING
#include <windows.h>
#endif

using namespace std;

_CSE_BEGIN

CSELog Log_IS;

_SC_BEGIN

string parser::SkipComments(string input)
{
	string Output;

	enum Comments
	{
		NoComment,
		SingleLine,
		MultiLine,
		InString
	};

	Comments S = NoComment;

	for (size_t i = 0; i < input.size() - 1; i++)
	{
		if ('\"' == input[i] && S == NoComment)
		{
			S = InString;
		}
		else if ('\"' == input[i] && S == InString)
		{
			S = NoComment;
		}

		if ('/' == input[i])
		{
			if ("//" == input.substr(i, 2) && S == NoComment)
			{
				S = SingleLine;
				continue;
			}
			else if ("/*" == input.substr(i, 2) && S == NoComment)
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

		if (S == NoComment || S == InString)
		{
			Output.push_back(input[i]);
		}
	}

	return Output;
}

template<typename _Iterator> requires random_access_iterator<_Iterator>
void ConsumeWhiteSpace(_Iterator& it, const _Iterator& end)
{
	while (it != end && isspace(*it))
	{
		++it;
	}
}

std::string parser::ParseValue(string::iterator& it, const string::iterator& end)
{
	string Value;

	enum state
	{
		Common,
		InString,
		InVector,
	}S = Common;

	while (it != end && !(isspace(*it) && Common == S))
	{
		if ('\"' == *it && Common == S)
		{
			S = InString;
			++it;
			continue;
		}
		else if ('\"' == *it && InString == S)
		{
			S = Common;
			++it;
			continue;
		}

		if ('(' == *it && Common == S)
		{
			S = InVector;
			continue;
		}
		else if (')' == *it && InVector == S)
		{
			S = Common;
			continue;
		}

		Value += *it;
		++it;
	}

	return Value;
}

int TrueOrFalse(std::string::iterator& it, const std::string::iterator& end)
{
	std::string Value;
	while (it != end && (!isspace(*it) && '(' != *it && '\"' != *it))
	{
		Value += *it;
		++it;
	}
	if (Value == "true") { return 1; }
	if (Value == "false") { return -1; }
	else { return 0; }
}

std::shared_ptr<table> parser::ParseSubTable(std::string::iterator& it, const std::string::iterator& end)
{
	uint64 SubTableLayer = 0;
	string SubStr;
	table SubTable;

	while (it != end)
	{
		if ('{' == *it)
		{
			++SubTableLayer;
			if (SubTableLayer != 1){ SubStr += *it; }
			++it;
			continue;
		}
		if ('}' == *it)
		{
			if (SubTableLayer != 1) { SubStr += *it; }
			--SubTableLayer;
			++it;
			continue;
		}
		if (SubTableLayer == 0 || it == end) { break; }
		SubStr += *it;
		++it;
	}
	
	auto it2 = SubStr.begin();
	auto end2 = SubStr.end();
	ConsumeWhiteSpace(it2, end2);

	while (it2 != end2)
	{
		table::KeyValue Data;
		// Parse Key
		while (it2 != end2 && !isspace(*it2) && '(' != *it2 && '\"' != *it2)
		{
			Data.Key += *it2;
			++it2;
		}

		ConsumeWhiteSpace(it2, end2);
		
		// Parse Value and SubTable
		uint64 NValues = 0;
		auto it2tof = it2;
		while (it2 != end2 && '{' != *it2 && (!isalpha(*it2) || TrueOrFalse(it2tof, end2)))
		{
			if (NValues)
			{
				if (Data.SubTable == nullptr)
				{
					table Tbl;
					Data.SubTable = make_shared<table>(Tbl);
				}
				Data.SubTable->push({ .Key = "UnnamedValue#" + std::to_string(NValues), .Value = ParseValue(it2, end2) });
			}
			else { Data.Value = ParseValue(it2, end2); };
			if (it2 != end2){++it2;}
			++NValues;
			ConsumeWhiteSpace(it2, end2);
		}
		
		ConsumeWhiteSpace(it2, end2);

		if (it2 != end2 && '{' == *it2)
		{
			auto it2a = it2;
			++it2a;
			ConsumeWhiteSpace(it2a, end2);
			if (it2a != end2 && (isdigit(*it2a) || '+' == *it2a || '-' == *it2a || '\"' == *it2a || '(' == *it2a))
			{
				if (!NValues) { Data.Value = ParseMatrix(it2, end2); }
				else
				{
					if (Data.SubTable == nullptr)
					{
						table x;
						Data.SubTable = make_shared<table>(x);
					}
					std::string Value = ParseMatrix(it2, end2);
					Data.SubTable->push({ .Key = "UnnamedArray", .Value = Value });
				}
			}
			else { Data.SubTable = ParseSubTable(it2, end2); }
		}

		SubTable.push(Data);
		Data.Key.clear();
		Data.Value.clear();

		ConsumeWhiteSpace(it2, end2);
	}

	return make_shared<table>(SubTable);
}

string parser::ParseMatrix(string::iterator& it, const string::iterator& end)
{
	string Value;
	bool InString = false;

	while (it != end)
	{
		if (*it == '\"' && !InString) { InString = true; }
		else if (*it == '\"' && InString) { InString = false; }
		if (*it == '{' && !InString)
		{ 
			++it;
			continue; 
		}
		Value += *it;
		++it;
		if (it == end) { break; }
		if (*it == '}' && !InString)
		{ 
			if (it != end) { ++it; }
			break; 
		}
	}

	return Value;
}

#if FORCE_CONVERT_CHAR_ENCODING
string parser::ConvertChar(const char* str, int SrcEncod)
{
	// This function taken from CSDN jianminfly
	// https://blog.csdn.net/jianminfly/article/details/106186909
	// Copyright(C) jianminfly，Licenced under CC BY-SA 4.0
	Log_IS.Out("ISCStream", "WARNING", "[Parser] U8 version of parser is developing, and CSE will completely support UTF-8 in the future versions.", ILogLevel, true);

	string result;
	WCHAR* strSrc;
	LPSTR szRes;

	// Get size of variable
	int i = MultiByteToWideChar(SrcEncod, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(SrcEncod, 0, str, -1, strSrc, i);

	// Get size of variable
	i = WideCharToMultiByte(TARGET_ENCODING, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(TARGET_ENCODING, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete[]strSrc;
	delete[]szRes;

	return result;
}
#endif

table parser::parse(UINT CodePage)
{
	Log_IS.Out("ISCStream", "INFO", "[Parser] Loading - Preprocessing catalog file...", ILogLevel, true);
	Log_IS.Out("ISCStream", "INFO", "[Parser] Using localization setting: " + LocalInfo, ILogLevel, true);
	ostringstream SCString;
	SCString << input.rdbuf();
	string SCPreProc = SkipComments(SCString.str());
	#if FORCE_CONVERT_CHAR_ENCODING
	SCPreProc = ConvertChar(SCPreProc.c_str(), CodePage);
	#endif
	
	table Catalogue;

	Log_IS.Out("ISCStream", "INFO", "[Parser] Loading - Initializing objects", ILogLevel, true);

	auto it = SCPreProc.begin();
	auto end = SCPreProc.end();
	ConsumeWhiteSpace(it, end);

	while (it != end)
	{
		table::KeyValue Data;
		// Parse Key
		while (it != end && !isspace(*it) && '(' != *it && '\"' != *it)
		{
			Data.Key += *it;
			++it;
		}

		ConsumeWhiteSpace(it, end);

		// Parse Value and SubTable
		uint64 NValues = 0;
		auto ittof = it;
		while (it != end && '{' != *it && (!isalpha(*it) || TrueOrFalse(ittof, end)))
		{
			if (NValues)
			{
				if (Data.SubTable == nullptr)
				{
					table Tbl;
					Data.SubTable = make_shared<table>(Tbl);
				}
				Data.SubTable->push({.Key = "UnnamedValue#" + std::to_string(NValues), .Value = ParseValue(it, end) });
			}
			else { Data.Value = ParseValue(it, end); }
			if(it != end){ ++it; }
			++NValues;
			ConsumeWhiteSpace(it, end);
		}
		
		ConsumeWhiteSpace(it, end);

		if (it != end && '{' == *it)
		{
			auto it2a = it;
			++it2a;
			ConsumeWhiteSpace(it2a, end);
			if (it2a != end && (isdigit(*it2a) || '+' == *it2a || '-' == *it2a || '\"' == *it2a || '(' == *it2a))
			{
				if (!NValues) { Data.Value = ParseMatrix(it, end); }
				else
				{
					if (Data.SubTable == nullptr)
					{
						table x;
						Data.SubTable = make_shared<table>(x);
					}
					std::string Value = ParseMatrix(it, end);
					Data.SubTable->push({ .Key = "UnnamedArray", .Value = Value });
				}
			}
			else { Data.SubTable = ParseSubTable(it, end); }
		}
		
		Catalogue.push(Data);
		Data.Key.clear();
		Data.Value.clear();

		ConsumeWhiteSpace(it, end);
	}

	Log_IS.Out("ISCStream", "INFO", "[Parser] " + to_string(Catalogue.Catalogue.size()) + " Objects(Keys) loaded.", ILogLevel, true);
	return Catalogue;
}

vector<_STD shared_ptr<table>> GetMultipleSubTables(vector<table::KeyValue>::iterator& _Iter, const vector<table::KeyValue>::iterator& _End, const string _Key)
{
	vector<shared_ptr<table>> _TList;
	while (_Iter != _End)
	{
		if (_Iter->Key == _Key)
		{
			_TList.push_back(_Iter->SubTable);
		}
		++_Iter;
	}
	return _TList;
}

_SC_END

ISCStream ParseFile(string FileName, UINT CodePage)
{
	Log_IS.Out("ISCStream", "INFO", "[Parser] Loading file \"" + FileName + "\"", ILogLevel, true);
	ifstream File(FileName);
	_CSE_VERIFY(File, _SC ParseException("File \"" + FileName + "\" is not found."));
	_SC parser se(File);
	return make_shared<_SC table>(se.parse(CodePage));
}

_CSE_END