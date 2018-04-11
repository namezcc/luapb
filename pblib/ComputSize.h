#ifndef COMPUT_SIZE_H
#define COMPUT_SIZE_H

#include "define.h"
#include <stdint.h>

const static int LittleEndian64Size = 8;
const static int LittleEndian32Size = 4;

LAPI static uint64_t EncodeZigZag64(int64_t n)
{
	return (uint64_t)((n << 1) ^ (n >> 63));
}

LAPI static uint32_t EncodeZigZag32(int n)
{
	return (uint32_t)((n << 1) ^ (n >> 31));
}

LAPI static int ComputeDouble(double v)
{
	return LittleEndian64Size;
}

LAPI static int ComputeFloat(float v)
{
	return LittleEndian32Size;
}

LAPI static int ComputeRawVarint32Size(uint32_t value)
{
	if ((value & (0xffffffff << 7)) == 0)
	{
		return 1;
	}
	if ((value & (0xffffffff << 14)) == 0)
	{
		return 2;
	}
	if ((value & (0xffffffff << 21)) == 0)
	{
		return 3;
	}
	if ((value & (0xffffffff << 28)) == 0)
	{
		return 4;
	}
	return 5;
}

LAPI static int ComputeRawVarint64Size(uint64_t value)
{
	if ((value & (0xffffffffffffffffL << 7)) == 0)
	{
		return 1;
	}
	if ((value & (0xffffffffffffffffL << 14)) == 0)
	{
		return 2;
	}
	if ((value & (0xffffffffffffffffL << 21)) == 0)
	{
		return 3;
	}
	if ((value & (0xffffffffffffffffL << 28)) == 0)
	{
		return 4;
	}
	if ((value & (0xffffffffffffffffL << 35)) == 0)
	{
		return 5;
	}
	if ((value & (0xffffffffffffffffL << 42)) == 0)
	{
		return 6;
	}
	if ((value & (0xffffffffffffffffL << 49)) == 0)
	{
		return 7;
	}
	if ((value & (0xffffffffffffffffL << 56)) == 0)
	{
		return 8;
	}
	if ((value & (0xffffffffffffffffL << 63)) == 0)
	{
		return 9;
	}
	return 10;
}

LAPI static int ComputeInt32Size(int value)
{
	if (value >= 0)
	{
		return ComputeRawVarint32Size((uint32_t)value);
	}
	else
	{
		// Must sign-extend.
		return 10;
	}
}

LAPI static int ComputeInt64Size(uint64_t value)
{
	return ComputeRawVarint64Size(value);
}

LAPI static int ComputeUInt32Size(uint32_t value)
{
	return ComputeRawVarint32Size(value);
}

LAPI static int ComputeUInt64(uint64_t v)
{
	return ComputeRawVarint64Size(v);
}

LAPI static int ComputeSInt64Size(uint64_t value)
{
	return ComputeRawVarint64Size(EncodeZigZag64(value));
}

LAPI static int ComputeSInt32Size(uint32_t value)
{
	return ComputeRawVarint32Size(EncodeZigZag32(value));
}

LAPI static int ComputeBoolSize(bool value)
{
	return 1;
}

LAPI static int ComputeLengthSize(int length)
{
	return ComputeRawVarint32Size((uint32_t)length);
}

LAPI static int ComputeStringSize(int slen)
{
	return ComputeLengthSize(slen) + slen;
}



#endif