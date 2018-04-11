#ifndef DECODE_VALUE_H
#define DECODE_VALUE_H

#include "define.h"
#include <stdlib.h>
#include <stdint.h>

LAPI static int ReadRawByte(OutPutStream* str,char& res)
{
	if (str->pos >= str->curLimit)
	{
		return -1;
	}
	res = str->buf[str->pos++];
	return 0;
}

LAPI static int ReadRawVarint32(OutPutStream* str, uint32_t& res)
{
	int len = str->curLimit - str->pos;
	if (len > 5)
		len = 5;
	int shift = 0;
	res = 0;
	for (size_t i = 0; i < len; i++)
	{
		int b = str->buf[str->pos++];
		res |= (b & 0x7F) << shift;
		if (b < 0x80)
			return 0;
		shift += 7;
	}
	return -1;
}

LAPI static int ReadTag(OutPutStream* str,uint32_t& res)
{
	if (str->hashNextTag)
	{
		str->lastTag = str->nextTag;
		str->hashNextTag = false;
		res = str->lastTag;
		return 0;
	}

	if(str->pos<str->curLimit)
	{
		int r = ReadRawVarint32(str, res);
		str->lastTag = res;
		return r;
	}
	else
	{
		str->lastTag = res = 0;
		return 0;
	}
	return 0;
}

LAPI static int ReadRawLittleEndian32(OutPutStream* str,uint32_t& res)
{
	if (str->pos + 4 > str->curLimit)
		return -1;

	res = str->buf[str->pos++];
	res |= (str->buf[str->pos++]) << 8;
	res |= (str->buf[str->pos++]) << 16;
	res |= (str->buf[str->pos++]) << 24;
	return 0;
}

/// <summary>
/// Reads a 64-bit little-endian integer from the stream.
/// </summary>
LAPI static int ReadRawLittleEndian64(OutPutStream* str, uint64_t& res)
{
	if (str->pos + 8 > str->curLimit)
		return -1;

	res = str->buf[str->pos++];
	res |= uint64_t(str->buf[str->pos++]) << 8;
	res |= uint64_t(str->buf[str->pos++]) << 16;
	res |= uint64_t(str->buf[str->pos++]) << 24;
	res |= uint64_t(str->buf[str->pos++]) << 32;
	res |= uint64_t(str->buf[str->pos++]) << 40;
	res |= uint64_t(str->buf[str->pos++]) << 48;
	res |= uint64_t(str->buf[str->pos++]) << 56;
	return 0;
}

LAPI static int ReadDouble(OutPutStream* str, double& res)
{
	return ReadRawLittleEndian64(str,*(uint64_t*)&res);
}

LAPI static int ReadFloat(OutPutStream* str, float& res)
{
	return ReadRawLittleEndian32(str, *(uint32_t*)&res);
}

LAPI static int ReadRawVarint64(OutPutStream* str,uint64_t& res)
{
	int len = str->curLimit - str->pos;
	if (len > 10)
		len = 10;
	int shift = 0;
	res = 0;
	for (size_t i = 0; i < len; i++)
	{
		char b = str->buf[str->pos++];
		res |= (uint64_t)(b & 0x7F) << shift;
		if (b < 0x80)
			return 0;
		shift += 7;
	}
	return -1;
}

LAPI static int ReadUInt64(OutPutStream* str,uint64_t& res)
{
	return ReadRawVarint64(str, res);
}

LAPI static int ReadInt64(OutPutStream* str, int64_t& res)
{
	return ReadRawVarint64(str, *(uint64_t*)&res);
}

LAPI static int ReadUInt32(OutPutStream* str, uint32_t& res)
{
	return ReadRawVarint32(str, res);
}

LAPI static int ReadInt32(OutPutStream* str, int32_t& res)
{
	return ReadRawVarint32(str, *(uint32_t*)&res);
}

LAPI static int ReadLength(OutPutStream* str,int& len)
{
	return ReadRawVarint32(str, *(uint32_t*)&len);
}

LAPI static int ReadString(OutPutStream* str,char* &res,int& len)
{
	int err = ReadLength(str, len);
	if (err < 0)
		return -1;
	if (len == 0)
		return -1;
	if (str->pos + len > str->curLimit)
		return -1;
	res = (char*)malloc(len);
	for (size_t i = 0; i < len; i++)
	{
		res[i] = str->buf[str->pos + i];
	}
	str->pos += len;
	return 0;
}

#endif