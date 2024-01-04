//
//  envar.cpp
//
//  Created by skorokhodov on 2023/11/18.
//

#include <stdlib.h>
#include <utils/envar.h>

#ifdef IS_WINDOWS
int setenv(const char* name, const char* value, int overwrite)
{
	if (overwrite)
		return _putenv_s(name, value);
	else // Just check if the variable exists
	{
		char* env = nullptr;
		size_t len = 0;
		errno_t err = _dupenv_s(&env, &len, name);
		if (env)
			free(env);
		if (err)
			return err;
		return 0;
	}	
}

int unsetenv(const char* name)
{
	return setenv(name, "");
}
#endif
