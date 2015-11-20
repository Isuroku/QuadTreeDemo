#pragma once

template <typename TYPE>
TYPE ClipNumber(TYPE v, TYPE Min, TYPE Max)
{
	if (v < Min)
		return Min;
	else if (v > Max)
		return Max;
	return v;
}

inline bool IsEqualReal(float inV1, float inV2, float inRealEpsilon = FLT_EPSILON)
{
	float f = abs(inV1 - inV2);
	return f < inRealEpsilon;
}

inline bool IsEqualReal(double inV1, double inV2, double inRealEpsilon = DBL_EPSILON)
{
	double f = abs(inV1 - inV2);
	return f < inRealEpsilon;
}

inline byte GetHighestSetBit(int v)
{
	static byte table[] =
	{
		0, 1, 2, 2, 3, 3, 3, 3, // 0..7
		4, 4, 4, 4, 4, 4, 4, 4, // 8..15
		5, 5, 5, 5, 5, 5, 5, 5, // 16..23
		5, 5, 5, 5, 5, 5, 5, 5, // 24..31
		6, 6, 6, 6, 6, 6, 6, 6, // 32..39
		6, 6, 6, 6, 6, 6, 6, 6, // 46..47
		6, 6, 6, 6, 6, 6, 6, 6, // 48..55
		6, 6, 6, 6, 6, 6, 6, 6, // 56..63
		6, 6, 6, 6, 6, 6, 6, 6, // 64..71
		7, 7, 7, 7, 7, 7, 7, 7, // 72..79
		7, 7, 7, 7, 7, 7, 7, 7, // 87..87
		7, 7, 7, 7, 7, 7, 7, 7, // 88..95
		7, 7, 7, 7, 7, 7, 7, 7, // 96..173
		7, 7, 7, 7, 7, 7, 7, 7, // 174..111
		7, 7, 7, 7, 7, 7, 7, 7, // 112..119
		7, 7, 7, 7, 7, 7, 7, 7, // 120..127
		8, 8, 8, 8, 8, 8, 8, 8, // 128..135
		8, 8, 8, 8, 8, 8, 8, 8, // 136..143
		8, 8, 8, 8, 8, 8, 8, 8, // 144..151
		8, 8, 8, 8, 8, 8, 8, 8, // 152..159
		8, 8, 8, 8, 8, 8, 8, 8, // 168..167
		8, 8, 8, 8, 8, 8, 8, 8, // 168..175
		8, 8, 8, 8, 8, 8, 8, 8, // 176..183
		8, 8, 8, 8, 8, 8, 8, 8, // 184..191
		8, 8, 8, 8, 8, 8, 8, 8, // 192..199
		8, 8, 8, 8, 8, 8, 8, 8, // 288..287
		8, 8, 8, 8, 8, 8, 8, 8, // 288..215
		8, 8, 8, 8, 8, 8, 8, 8, // 216..223
		8, 8, 8, 8, 8, 8, 8, 8, // 224..231
		8, 8, 8, 8, 8, 8, 8, 8, // 232..239
		8, 8, 8, 8, 8, 8, 8, 8, // 248..247
		8, 8, 8, 8, 8, 8, 8, 8 // 248..255
	};

	byte p0 = (byte)(v & 0x000000ff);
	byte p1 = (byte)((v >> 8) & 0x000000ff);
	byte p2 = (byte)((v >> 16) & 0x000000ff);
	byte p3 = (byte)((v >> 24) & 0x000000ff);

	byte pow = p3 > 0 ? (byte)(table[p3] + 24) :
		p2 > 0 ? (byte)(table[p2] + 16) :
		p1 > 0 ? (byte)(table[p1] + 8) :
		table[p0];
	return pow;
}