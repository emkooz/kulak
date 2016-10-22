#include "cvar.hpp"

namespace kk
{
	cvar* cvars;

	cvar* cvarGet(const std::string& name)
	{
		for (cvar* var = cvars; var; var = var->next_cvar)
		{
			if (name == var->name)
				return var;
		}

		return NULL;
	}

	// Usually only called by the user, cvars should be global-ish
	void cvarSet(const std::string& name, const std::string& value)
	{
		cvar* var = cvarGet(name);

		if (var != NULL)
		{
			try 
			{
				var->value = std::stof(value);
			}
			catch (const std::invalid_argument& ia)
			{
				kk::log (logBasic{value + "is invalid input for " + name, kk::logType::warning});
			}
		}
	}

	// Adds the cvar to the list
	void cvarRegister(cvar &var)
	{
		if (cvarGet(var.name) == NULL)
		{
			var.next_cvar = cvars;
			cvars = &var;

			kk::log ("Registered cvar: " + var.name);
		}
		else
		{
			kk::log (logBasic{var.name + " already exists!", kk::logType::warning});
		}
	}
}