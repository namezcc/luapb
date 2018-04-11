#ifndef DEFINE_H
#define DEFINE_H

#define LAPI 

enum Stream_Type
{
	S_ENCODE,
	S_DECODE,
};

struct OutPutStream
{
	char* buf;
	int len;
	int pos;
	int lastTag;
	int nextTag;
	int curLimit;
	bool hashNextTag;
	int type;
};

union LittleEndian
{
	int i;
	char a;
};

constexpr LittleEndian _checkEndian{1};

LAPI static bool IsLittleEndian()
{
	return _checkEndian.a;
}

#endif