//
//  envar.h
//
//  Created by skorokhodov on 2023/11/18.
//

#pragma once

#include <utils/system_macros.h>

#ifdef IS_WINDOWS
int setenv(const char* name, const char* value, int overwrite = 1);
int unsetenv(const char* name);
#else
#include <stdlib.h>
#endif
