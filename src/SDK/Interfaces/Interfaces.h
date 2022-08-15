#pragma once

#include "../EntityCache/EntityCache.h"

#define XASSERTFALSE(x) if (x) { Error(#x); return false; }

class CInterfaces
{
public:
	bool Initialize();
};

inline CInterfaces g_Interfaces;