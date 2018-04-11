#include "BindLpb.h"

int luaopen_pb(lua_State * L)
{
	luaL_Reg reg[] = {
		{"clearStream",pb_clearStream},
		{ "newStream",pb_newStream },
		{ "sizeInt",pb_sizeInt },
		{ "sizeUInt",pb_sizeUInt },
		{ "sizeInt64",pb_sizeInt64 },
		{ "sizeUInt64",pb_sizeUInt64 },
		{ "sizeFloat",pb_sizeFloat },
		{ "sizeDouble",pb_sizeDouble },
		{ "sizeString",pb_sizeString },
		{ "sizeTag",pb_sizeTag },
		{ "wBool",pb_wBool },
		{ "wDouble",pb_wDouble },
		{ "wFloat",pb_wFloat },
		{ "wInt",pb_wInt},
		{ "wInt64",pb_wInt64 },
		{ "wString",pb_wString},
		{ "wUInt",pb_wUint },
		{ "wUInt64",pb_wUint64 },
		{ "rBool",pb_rBool },
		{ "rDouble",pb_rDouble },
		{ "rFloat",pb_rFloat },
		{ "rInt",pb_rInt },
		{ "rInt64",pb_rInt64 },
		{ "rString",pb_rString },
		{ "rUInt",pb_rUint },
		{ "rUInt64",pb_rUint64 },
		{ "rTag",pb_rTag },
		{ "MaybeConsumeTag",pb_MaybeConsumeTag },
		{ "PushLismit",pb_PushLimit },
		{ "IsEndLimit",pb_IsEndLimit },
		{ "PopLimit",pb_PopLimit },
		{ "toStringbyte",pb_toStringByte },
		{ "fromString",pb_fromString },
		{ "sleep",pb_Sleep },
		{NULL,NULL},
	};

	luaL_Reg regmeta[] = {
		{"__gc",pb_clearStream },
		{ NULL,NULL },
	};

	luaL_newmetatable(L, "OutPutStream");
	luaL_setfuncs(L, regmeta,0);

	luaL_newlib(L, reg);
	return 1;
}
