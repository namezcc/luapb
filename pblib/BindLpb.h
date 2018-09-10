#ifndef BIND_LPB_H
#define BIND_LPB_H

#include "DecodeValue.h"
#include "EncodeValue.h"
#include "ComputSize.h"
#include "lua.hpp"

static int pb_sizeTag(lua_State* L)
{
	int v = lua_tointeger(L, 1);
	int s = ComputeInt32Size(v);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeInt(lua_State* L)
{
	int v = lua_tointeger(L, 1);
	int s = ComputeInt32Size(v);
	lua_settop(L, 0);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeUInt(lua_State* L)
{
	uint32_t v = lua_tointeger(L, 1);
	int s = ComputeUInt32Size(v);
	lua_settop(L, 0);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeInt64(lua_State* L)
{
	int64_t v = lua_tointeger(L, 1);
	int s = ComputeInt64Size(v);
	lua_settop(L, 0);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeUInt64(lua_State* L)
{
	uint64_t v = lua_tointeger(L, 1);
	int s = ComputeUInt64(v);
	lua_settop(L, 0);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeFloat(lua_State* L)
{
	float v = lua_tonumber(L, 1);
	int s = ComputeFloat(v);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeDouble(lua_State* L)
{
	double v = lua_tonumber(L, 1);
	int s = ComputeDouble(v);
	lua_pushinteger(L, s);
	return 1;
}

static int pb_sizeString(lua_State* L)
{
	size_t len;
	const char* v = lua_tolstring(L, 1,&len);
	int s = ComputeStringSize(len);
	lua_pushinteger(L, s);
	return 1;
}

static OutPutStream* pb_getStream(lua_State* L)
{
	void* ud = luaL_checkudata(L, 1,"OutPutStream");
	if (ud == NULL)
	{
		luaL_error(L, "pb Stream is nil");
		return NULL;
	}
	return *(OutPutStream**)ud;
}

static int pb_clearStream(lua_State* L)
{
	OutPutStream** ud = (OutPutStream**)luaL_checkudata(L, 1,"OutPutStream");
	if (ud == NULL)
	{
		luaL_error(L, "pb Stream is nil");
		return 0;
	}
	//OutPutStream** 
	OutPutStream* s = *ud;
	if (s)
	{
		if (s->buf)
		{
			if(s->type==S_ENCODE)
				free(s->buf);
			s->buf = NULL;
		}
		free(s); //
		*ud = NULL;
	}
	return 0;
}

static int pb_toStringByte(lua_State* L)
{
	OutPutStream* s = pb_getStream(L);
	if (s)
	{
		lua_pushlstring(L, s->buf, s->len);
		return 1;
	}
	return 0;
}

static int pb_Sleep(lua_State* L)
{
	int n = lua_tointeger(L, 1);
	_sleep(n);
	return 0;
}

static int pb_fromString(lua_State* L)
{
	size_t len;
	const char* buf = lua_tolstring(L, 1, &len);

	OutPutStream** udata = (OutPutStream**)lua_newuserdata(L, sizeof(OutPutStream*));
	*udata = (OutPutStream*)malloc(sizeof(OutPutStream));
	luaL_getmetatable(L, "OutPutStream");
	lua_setmetatable(L, -2);
	OutPutStream* pbstream = *udata;
	pbstream->buf = (char*)buf;
	pbstream->pos = 0;
	pbstream->len = len;
	pbstream->curLimit = len;
	pbstream->lastTag = 0;
	pbstream->nextTag = 0;
	pbstream->hashNextTag = false;
	pbstream->type = S_DECODE;
	return 1;
}

static int pb_newStream(lua_State* L)
{
	int len = lua_tointeger(L, 1);
	if (len == 0)
		return 0;

	OutPutStream** udata = (OutPutStream**)lua_newuserdata(L, sizeof(OutPutStream*));
	*udata = (OutPutStream*)malloc(sizeof(OutPutStream));
	luaL_getmetatable(L, "OutPutStream");
	lua_setmetatable(L, -2);
	OutPutStream* pbstream = *udata;
	pbstream->buf = (char*)malloc(len);
	pbstream->pos = 0;
	pbstream->len = len;
	pbstream->curLimit = len;
	pbstream->lastTag = 0;
	pbstream->nextTag = 0;
	pbstream->hashNextTag = false;
	pbstream->type = S_ENCODE;
	//lua_pushlightuserdata(L, pbstream);
	return 1;
}

static int pb_wInt(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int v = lua_tointeger(L, 2);
	WriteInt32(pbstream, v);
	return 0;
}

static int pb_wInt64(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int64_t v = lua_tointeger(L, 2);
	WriteInt64(pbstream, v);
	return 0;
}

static int pb_wUint(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	uint32_t v = lua_tointeger(L, 2);
	WriteUInt32(pbstream,v);
	return 0;
}

static int pb_wUint64(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int64_t v = lua_tointeger(L, 2);
	WriteUInt64(pbstream, v);
	return 0;
}

static int pb_wDouble(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	double v = lua_tonumber(L, 2);
	WriteDouble(pbstream, v);
	return 0;
}

static int pb_wFloat(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	float v = lua_tonumber(L, 2);
	WriteFloat(pbstream, v);
	return 0;
}

static int pb_wBool(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int v = lua_toboolean(L, 2);
	WriteInt32(pbstream, v);
	return 0;
}

static int pb_wString(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	size_t len;
	const char* v = lua_tolstring(L, 2,&len);
	WriteString(pbstream,(char*)v,len);
	return 0;
}

static int pb_rTag(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	uint32_t v;
	ReadTag(pbstream, &v);
	lua_pushinteger(L, v);
	return 1;
}

static int pb_rInt(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int v;
	ReadInt32(pbstream, &v);
	lua_pushinteger(L, v);
	return 1;
}

static int pb_rInt64(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int64_t v;
	ReadInt64(pbstream, &v);
	lua_pushinteger(L, v);
	return 1;
}

static int pb_rUint(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	uint32_t v;
	ReadUInt32(pbstream, &v);
	lua_pushinteger(L, v);
	return 1;
}

static int pb_rUint64(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	uint64_t v;
	ReadUInt64(pbstream, &v);
	lua_pushinteger(L, v);
	return 1;
}

static int pb_rDouble(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	double v;
	ReadDouble(pbstream, &v);
	lua_pushnumber(L, v);
	return 1;
}

static int pb_rFloat(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	float v;
	ReadFloat(pbstream, &v);
	lua_pushnumber(L, v);
	return 1;
}

static int pb_rBool(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int v;
	ReadInt32(pbstream, &v);
	lua_pushboolean(L, v);
	return 1;
}

static int pb_rString(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int v;
	char* s=NULL;
	ReadString(pbstream, &s,&v);
	if (s)
	{
		lua_pushlstring(L, s, v);
		free(s);
		return 1;
	}
	return 0;
}

static int PeekTag(OutPutStream* str)
{
	if (str->hashNextTag)
	{
		return str->nextTag;
	}
	uint32_t tag;
	ReadTag(str, &tag);
	int saveLast = str->lastTag;
	str->nextTag = tag;
	str->hashNextTag = true;
	str->lastTag = saveLast;
	return tag;
}

static int pb_MaybeConsumeTag(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int tag = lua_tointeger(L, 2);

	if (PeekTag(pbstream) == tag)
	{
		pbstream->hashNextTag = false;
		lua_pushboolean(L, 1);
	}else
		lua_pushboolean(L, 0);
	return 1;
}

static int pb_PushLimit(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int limit = lua_tointeger(L, 2);
	int oldlimit = pbstream->curLimit;
	limit += pbstream->pos;
	if (limit > oldlimit)
		return 0;

	pbstream->curLimit = limit;
	lua_pushinteger(L, oldlimit);
	return 1;
}

static int pb_IsEndLimit(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;

	if (pbstream->pos >= pbstream->curLimit)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);
	return 1;
}

static int pb_PopLimit(lua_State* L)
{
	OutPutStream* pbstream = pb_getStream(L);
	if (!pbstream)
		return 0;
	int limit = lua_tointeger(L, 2);
	if (limit > pbstream->curLimit && limit <= pbstream->len)
		pbstream->curLimit = limit;
	return 0;
}

extern "C" _declspec(dllexport) int luaopen_pb(lua_State* L);

#endif