#ifndef ENCODE_VALUE_H
#define ENCODE_VALUE_H

#include "define.h"
#include <stdint.h>

LAPI static bool WriteRawLittleEndian64(OutPutStream* stream,uint64_t v)
{
	if (stream->pos + 8 > stream->len)
		return false;
	stream->buf[stream->pos++] = ((char) v);
	stream->buf[stream->pos++] = ((char) (v >> 8));
	stream->buf[stream->pos++] = ((char)(v >> 16));
	stream->buf[stream->pos++] = ((char)(v >> 24));
	stream->buf[stream->pos++] = ((char)(v >> 32));
	stream->buf[stream->pos++] = ((char)(v >> 40));
	stream->buf[stream->pos++] = ((char)(v >> 48));
	stream->buf[stream->pos++] = ((char)(v >> 56));
	return true;
}

LAPI static bool WriteDouble(OutPutStream* str,double v)
{
	uint16_t* nv = (uint16_t*)&v;
	return WriteRawLittleEndian64(str, *nv);
}

LAPI static bool WriteFloat(OutPutStream* str, float v)
{
	if (str->pos + 4 > str->len)
		return false;

	char* nv = (char*)&v;

	str->buf[str->pos++] = nv[0];
	str->buf[str->pos++] = nv[1];
	str->buf[str->pos++] = nv[2];
	str->buf[str->pos++] = nv[3];
	return true;
}

LAPI static bool WriteRawVarint64(OutPutStream* str,uint64_t v)
{
	while (v > 127)
	{
		if (str->pos >= str->len)
			return false;
		str->buf[str->pos++] = (char)((v & 0x7F) | 0x80);
		v >>= 7;
	}
	if (str->pos >= str->len)
		return false;
	str->buf[str->pos++] = (char)v;
	return true;
}

LAPI static bool WriteUInt64(OutPutStream* str,uint64_t v)
{
	return WriteRawVarint64(str,v);
}

LAPI static bool WriteInt64(OutPutStream* str, int64_t value)
{
	return WriteRawVarint64(str,(uint64_t)value);
}

LAPI static bool WriteRawVarint32(OutPutStream* str, uint32_t v)
{
	while (v>127)
	{
		if (str->pos >= str->len)
			return false;
		str->buf[str->pos++] = (char)((v & 0x7F) | 0x80);
		v >>= 7;
	}
	if (str->pos >= str->len)
		return false;
	str->buf[str->pos++] = (char)v;
	return true;
}

LAPI static bool WriteInt32(OutPutStream* str, int value)
{
	if (value >= 0)
		return WriteRawVarint32(str,(uint32_t)value);
	else
		return WriteRawVarint64(str, (uint64_t)value);
}

LAPI static bool WriteLength(OutPutStream* str, int length)
{
	return WriteRawVarint32(str, (uint32_t)length);
}

LAPI static bool WriteString(OutPutStream* str,char* src,int len)
{
	if (!WriteLength(str, len))
		return false;

	if (str->pos + len > str->len)
		return false;

	for (size_t i = 0; i < len; i++)
	{
		str->buf[str->pos + i] = src[i];
	}
	str->pos += len;
	return true;
}

LAPI static bool WriteUInt32(OutPutStream* str, uint32_t v)
{
	return WriteRawVarint32(str,v);
}

LAPI static bool WriteTag(OutPutStream* str, uint32_t tag)
{
	return WriteRawVarint32(str,tag);
}

#endif