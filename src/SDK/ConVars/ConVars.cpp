#include "ConVars.h"

#define _CHECKVAR(x) if (x) { Error(#x); return false; }
#define CHECKVAR(x) _CHECKVAR(x == nullptr)

bool CConVars::Initialize()
{
	cl_interp = I::Cvar->FindVar("cl_interp");
	CHECKVAR(cl_interp);

	cl_interp_ratio = I::Cvar->FindVar("cl_interp_ratio");
	CHECKVAR(cl_interp_ratio);

	cl_updaterate = I::Cvar->FindVar("cl_updaterate");
	CHECKVAR(cl_updaterate);

	sv_gravity = I::Cvar->FindVar("sv_gravity");
	CHECKVAR(sv_gravity);

	cl_flipviewmodels = I::Cvar->FindVar("cl_flipviewmodels");
	CHECKVAR(cl_flipviewmodels);

	//If we arrive here, return success.
	return true;
}