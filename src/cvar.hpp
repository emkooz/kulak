#pragma once
#include <string>
#include <iostream>
#include "log.hpp"

// largely inspired by how Quake handles cvars
/*
Usage:
Create the CVar like so: kk::cvar sv_tickrate = { "sv_tickrate", 100 };
	- CVar must be created where it would not be destroyed before program termination.

Register the CVar to add it to the CVar list: kk::cvarRegister(sv_tickrate);

The CVar should be passed around directly, modifying it by doing something like:
sv_tickrate.value = 66;

cvarSet and cvarGet ideally should only be used when the user inputs console commands.
*/

namespace kk
{
	struct cvar
	{
		std::string name;
		float value;
		cvar* next_cvar;
	};

	extern cvar* cvars; // all cvars

	void cvarRegister(cvar &var);
	void cvarSet(const std::string& name, const std::string& value);
	cvar* cvarGet(const std::string& name);
}