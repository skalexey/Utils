// profiler.h

#pragma once

#include <chrono>
#include <utils/datetime.h>

//#define PROFILER_ON

#ifdef PROFILER_ON
#define PROFILE_DUR(expr) { \
	using namespace std::chrono; \
	auto start = steady_clock::now(); \
	expr; \
	auto end = steady_clock::now(); \
	LOG("Execution of expression '" << #expr << "' took " << duration_cast<milliseconds>(end - start).count() << "ms"); \
}

#define PROFILE_TIME(expr) { \
	using namespace std::chrono; \
	LOG("Expression '" << #expr << "' called at " << utils::time(true)); \
	expr; \
}
#else
#define PROFILE_DUR(expr) expr;
#define PROFILE_TIME(expr) expr;
#endif