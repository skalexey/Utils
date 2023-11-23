//
//  envar.cpp
//
//  Created by skorokhodov on 2023/11/18.
//

#include <stdlib.h>
#include <utils/envar.h>

#ifdef IS_WINDOWS
int setenv(const char* name, const char* value)
{
	return _putenv_s(name, value);
}

int unsetenv(const char* name)
{
	return setenv(name, "");
}
#endif
