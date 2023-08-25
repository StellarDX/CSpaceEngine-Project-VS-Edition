#include "CSE/Stars/StarBarycenter.h"

_CSE_BEGIN

// Coord in 24-h

UCoordinate24::UCoordinate24(float64 h)
{
	while (h < 0) { h += 24; }
	while (h >= 24) { h -= 24; }
	uint16_t Hours((uint16_t)h);
	float64 Minf((h - Hours) * 60);
	uint16_t Mins((uint16_t)Minf);
	float64 Secs((Minf - Mins) * 60);
	this->UCoordinate24::UCoordinate24(Hours, Mins, Secs);
}

UCoordinate24::UCoordinate24(uint64 h, uint64 m, float64 s)
{
	while (s >= 60)
	{
		++m;
		s -= 60;
	}

	while (m >= 60)
	{
		++h;
		m -= 60;
	}

	if (h >= 24) { h %= 24; }

	_Data.hrs = h;
	_Data.min = m;
	_Data.sec0 = __Float64(s).parts.msw >> 16;
	_Data.sec1 = __Float64(s).parts.msw & uint32_t(0x0000FFFF);
	_Data.sec2 = __Float64(s).parts.lsw >> 16;
	_Data.sec3 = __Float64(s).parts.lsw & uint32_t(0x0000FFFF);
}

uint64 UCoordinate24::hrs() const
{
	return _Data.hrs;
}

uint64 UCoordinate24::min() const
{
	return _Data.min;
}

float64 UCoordinate24::sec() const
{
	union { float64 f; uint16_t i[4]; } Buf;
	Buf.i[0] = _Data.sec3;
	Buf.i[1] = _Data.sec2;
	Buf.i[2] = _Data.sec1;
	Buf.i[3] = _Data.sec0;
	return Buf.f;
}

UCoordinate24::operator float64()
{
	return hrs() + min() / 60. + sec() / 3600.;
}

UCoordinate24 UCoordinate24::operator=(float64 h)
{
	return UCoordinate24(h);
}

_STD ostream& operator<<(_STD ostream& os, UCoordinate24 Coord)
{
	os << Coord.hrs() << ' ' << Coord.min() << ' ' << Coord.sec();
	return os;
}

// Coord in +-90-deg

Coordinate90::Coordinate90(float64 d)
{
	while (d >= 360) { d -= 360; }
	while (d <= -360) { d += 360; }
	int64 Pos = Quadrant(d);
	if (d < 0 && Pos >= 1 && Pos <= 4) { d += 360; }
	else if (d > 0 && Pos >= 4 && Pos <= 7) { d -= 360; }
	int64 sign = int64(sgn(d));
	if (sign == -1) { d = -d; }
	if (d > 90){d = 180 - d;}

	uint16_t Degs((uint16_t)d);
	float64 Minf((d - Degs) * 60);
	uint16_t Mins((uint16_t)Minf);
	float64 Secs((Minf - Mins) * 60);
	this->Coordinate90::Coordinate90(sign * Degs, Mins, Secs);
}

Coordinate90::Coordinate90(int64 d, uint64 m, float64 s)
{
	while (s >= 60)
	{
		++m;
		s -= 60;
	}

	while (m >= 60)
	{
		++d;
		m -= 60;
	}

	int64 D90 = d / 90;
	int64 BaseSign = sgn(d), Sign;
	if (d >= 360 || d <= -360) { d %= 360; }
	int64 Pos = Quadrant(float64(d));
	if (BaseSign == -1 && Pos >= 1 && Pos <= 4)
	{
		d += 360;
		Sign = 0;
	}
	else if (BaseSign >= 0 && Pos >= 4 && Pos <= 7)
	{
		d -= 360;
		Sign = 1;
	}
	else { Sign = BaseSign == -1; }
	//Pos = Quadrant(float64(d));

	d = abs(d);
	if (d > 90){d = 179 - d;}
	if (D90 % 2 != 0)
	{
		m = 59 - m;
		s = 60 - s;
	}

	_Data.neg = Sign;
	_Data.deg = d;
	_Data.min = m;
	_Data.sec0 = __Float64(s).parts.msw >> 16;
	_Data.sec1 = __Float64(s).parts.msw & uint32_t(0x0000FFFF);
	_Data.sec2 = __Float64(s).parts.lsw >> 16;
	_Data.sec3 = __Float64(s).parts.lsw & uint32_t(0x0000FFFF);
}

bool Coordinate90::neg() const
{
	return _Data.neg;
}

uint64 Coordinate90::deg() const
{
	return _Data.deg;
}

uint64 Coordinate90::min() const
{
	return _Data.min;
}

float64 Coordinate90::sec() const
{
	union { float64 f; uint16_t i[4]; } Buf;
	Buf.i[0] = _Data.sec3;
	Buf.i[1] = _Data.sec2;
	Buf.i[2] = _Data.sec1;
	Buf.i[3] = _Data.sec0;
	return Buf.f;
}

Coordinate90::operator float64()
{
	return deg() + min() / 60. + sec() / 3600.;
}

Coordinate90 Coordinate90::operator=(float64 h)
{
	return Coordinate90(h);
}

_STD ostream& operator<<(_STD ostream& os, Coordinate90 Coord)
{
	if (Coord.neg()) { os << '-'; }
	else { os << '+'; }
	os << Coord.deg() << ' ' << Coord.min() << ' ' << Coord.sec();
	return os;
}

// Coord in 360-deg

UCoordinate360::UCoordinate360(float64 d)
{
	while (d < 0) { d += 360; }
	while (d >= 360) { d -= 360; }
	uint16_t Degs((uint16_t)d);
	float64 Minf((d - Degs) * 60);
	uint16_t Mins((uint16_t)Degs);
	float64 Secs((Minf - Mins) * 60);
	this->UCoordinate360::UCoordinate360(Degs, Mins, Secs);
}

UCoordinate360::UCoordinate360(uint64 d, uint64 m, float64 s)
{
	while (s >= 60)
	{
		++m;
		s -= 60;
	}

	while (m >= 60)
	{
		++d;
		m -= 60;
	}

	if (d >= 360) { d %= 360; }

	_Data.deg = d;
	_Data.min = m;
	_Data.sec0 = __Float64(s).parts.msw >> 16;
	_Data.sec1 = __Float64(s).parts.msw & uint32_t(0x0000FFFF);
	_Data.sec2 = __Float64(s).parts.lsw >> 16;
	_Data.sec3 = __Float64(s).parts.lsw & uint32_t(0x0000FFFF);
}

uint64 UCoordinate360::deg() const
{
	return _Data.deg;
}

uint64 UCoordinate360::min() const
{
	return _Data.min;
}

float64 UCoordinate360::sec() const
{
	union { float64 f; uint16_t i[4]; } Buf;
	Buf.i[0] = _Data.sec3;
	Buf.i[1] = _Data.sec2;
	Buf.i[2] = _Data.sec1;
	Buf.i[3] = _Data.sec0;
	return Buf.f;
}

UCoordinate360::operator float64()
{
	return deg() + min() / 60. + sec() / 3600.;
}

UCoordinate360 UCoordinate360::operator=(float64 d)
{
	return UCoordinate360(d);
}

_STD ostream& operator<<(_STD ostream& os, UCoordinate360 Coord)
{
	os << Coord.deg() << ' ' << Coord.min() << ' ' << Coord.sec();
	return os;
}

_CSE_END
