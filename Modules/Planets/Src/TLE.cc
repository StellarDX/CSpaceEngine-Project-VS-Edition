#include "CSE/Planets/Orbit.h"

_CSE_BEGIN

// TLE Functions

// We only yearn for the skies because we cannot fly...  --Nahida

#ifdef _CRT_SECURE_NO_WARNINGS
TLE::TLE(_STD istream& _Input)
{
	_STD string Buffer;
	_STD getline(_Input, Buffer, '\n');
	strcpy(Name, Buffer.c_str());
	_STD getline(_Input, Buffer, '\n');
	strcpy(Line1, Buffer.c_str());
	_STD getline(_Input, Buffer, '\n');
	strcpy(Line2, Buffer.c_str());
}
#else
TLE::TLE(_STD istream& _Input)
{
	_STD string Buffer;
	_STD getline(_Input, Buffer, '\n');
	strcpy_s(Name, Buffer.c_str());
	_STD getline(_Input, Buffer, '\n');
	strcpy_s(Line1, Buffer.c_str());
	_STD getline(_Input, Buffer, '\n');
	strcpy_s(Line2, Buffer.c_str());
}
#endif

TLE TLE::ParseString(_STD string _Input)
{
	_STD istringstream _Is(_Input);
	return TLE(_Is);
}

bool TLE::isValid()const
{
	uint32_t Checksum1 = Line1[68] - '0';
	uint32_t Checksum2 = Line2[68] - '0';
	uint64 Sum1 = 0, Sum2 = 0;
	for (size_t i = 0; i < 68; i++)
	{
		if (isdigit(Line1[i])) { Sum1 += Line1[i] - '0'; }
		if (isdigit(Line2[i])) { Sum2 += Line2[i] - '0'; }
		if ('-' == Line1[i]) { Sum1 += 1; }
		if ('-' == Line2[i]) { Sum2 += 1; }
	}
	return Sum1 % 10 == Checksum1 && Sum2 % 10 == Checksum2;
}

typename TLE::TLE_Spacecraft TLE::BasicData() const
{
	using namespace std;
	string Buffer1(Line1);
	string Buffer2(Line2);

	auto FullYear = [](int32_t Yr)
	{
		if (Yr > 57 && Yr < 99) { return 1900 + Yr; }
		else if (Yr > 00 && Yr < 56) { return 2000 + Yr; }
		else { throw OrbitCalculateException("Invalid year number?"); }
	};

	auto Class = [](char C)->string
	{
		switch (C)
		{
		case 'U':
			return "Unclassified";
		case 'C':
			return "Classified";
		case 'S':
			return "Secret";
		default:
			throw OrbitCalculateException("Invalid classification?");
		}
	};

	return
	{
		.CatalogNumber = stoul(Buffer1.substr(2, 5)),
		.Classification = Class(Buffer1[7]),
		.IntDesignator = 
		{
			.LaunchYear = FullYear(stoi(Buffer1.substr(9, 2))),
			.LaunchNumber = stoul(Buffer1.substr(11, 3)),
			.LaunchPiece = Buffer1.substr(14, 3)
		},
		.BallisticCoeff = stod(Buffer1.substr(33, 10)),
		.d2MeanMotion = stod(Buffer1.substr(44, 6)) * pow(10, stod(Buffer1.substr(50, 2))),
		.BSTAR = stod(Buffer1.substr(53, 6)) * pow(10, stod(Buffer1.substr(59, 2))),
		.EphemerisType = stoul(Buffer1.substr(62, 1)),
		.ElementSet = stoul(Buffer1.substr(64, 4)),
		.RevolutionNum = stoul(Buffer2.substr(63, 5))
	};
}

OrbitParam TLE::Orbit() const
{
	using namespace std;
	string Buffer1(Line1);
	string Buffer2(Line2);

	auto FullYear = [](int32_t Yr)
	{
		if (Yr > 57 && Yr < 99) { return 1900 + Yr; }
		else if (Yr > 00 && Yr < 56) { return 2000 + Yr; }
		else { throw OrbitCalculateException("Invalid year number?"); }
	};

	return
	{
		.RefPlane = "Equator",
		.Epoch = _TIME CSEDate(FullYear(stoi(Buffer1.substr(18, 2))), 1, 1).toJulianDay() - 0.5 + stod(Buffer1.substr(20, 12)) - 1,
		.Period = SynodicDay / stod(Buffer2.substr(52, 11)),
		.Eccentricity = stod(Buffer2.substr(26, 7)) * 1E-7,
		.Inclination = stod(Buffer2.substr(8, 8)),
		.AscendingNode = stod(Buffer2.substr(17, 8)),
		.ArgOfPericenter = stod(Buffer2.substr(34, 8)),
		.MeanAnomaly = stod(Buffer2.substr(43, 8)),
	};
}

_STD string TLE::COSPAR()
{
	auto Data = BasicData();
	return _STD vformat("{}-{:03}{}", _STD make_format_args(Data.IntDesignator.LaunchYear, Data.IntDesignator.LaunchNumber, Data.IntDesignator.LaunchPiece));
}

void TLE::PrintSpacecraftData(::std::ostream& l1str)
{
	auto Data = BasicData();
	l1str << "Catalog Number:                   " << Data.CatalogNumber << '\n';
	l1str << "Classification:                   " << Data.Classification << '\n';
	l1str << "COSPAR ID:                        " << COSPAR() << '\n';
	l1str << "Ballistic Coefficient:            " << Data.BallisticCoeff << '\n';
	l1str << "Second Derivative of Mean Motion: " << Data.d2MeanMotion << '\n';
	l1str << "Radiation Rressure Coefficient:   " << Data.BSTAR << '\n';
	l1str << "Ephemeris Type:                   " << Data.EphemerisType << '\n';
	l1str << "Element Set Number:               " << Data.ElementSet << '\n';
	l1str << "Revolution number at epoch:       " << Data.RevolutionNum << '\n';
}

_CSE_END