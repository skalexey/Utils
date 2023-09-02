#pragma once

#if __cplusplus == 202101L
	#define CPPSTD 23
#elif __cplusplus == 202002L
	#define CPPSTD 20
#elif __cplusplus == 201703L
	#define CPPSTD 17
#elif __cplusplus == 201402L
	#define CPPSTD 14
#elif __cplusplus == 201103L
	#define CPPSTD 11
#elif __cplusplus == 199711L
	#define CPPSTD 98
#endif