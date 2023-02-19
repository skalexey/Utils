#pragma once

#include <iostream>

#ifndef COUT
	#define COUT(msg) std::cout << msg
#endif
#ifndef MSG
	#define MSG(msg) COUT(msg << "\n")
#endif
#ifndef LOG_ERROR
	#define LOG_ERROR(msg) MSG("ERROR: " << msg)
#endif