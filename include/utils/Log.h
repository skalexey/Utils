#pragma once

#ifdef LOG_ON
	#include <iostream>

	namespace
	{
		std::string logPrefix; // Prefix for LOG_* commands. Is set through LOG_PREFIX function
		std::string logPostfix; // Postfix for LOG_* commands. Is set through LOG_POSTFIX function

		auto logStream = []() -> std::ostream& {
			return std::cout;
		};

		struct PrefixInitializer
		{
			PrefixInitializer(const std::string& s) {
				logPrefix = s;
			}
		};

		struct PostfixInitializer
		{
			PostfixInitializer(const std::string& s) {
				logPostfix = s;
			}
		};
	}

	// Is necessarily required for all LOCAL_LOG* functions
	#define LOG_TITLE(title) namespace { const char* logTitle = title; } 
	#define LOG_STREAM(s) namespace {logStream = s;}
	#define SET_LOG_VERBOSE(boolVal) namespace { bool verboseLevel = boolVal;}
	#define SET_LOG_DEBUG(boolVal) namespace { bool debugLevel = boolVal; }
	#define LOCAL_LOG(msg) logStream() << "[" << logTitle << "] " << msg << "\n"
	#define LOCAL_DEBUG(msg) { if (debugLevel) { logStream() << "[" << logTitle << "] [D] " << msg << "\n"; } };
	#define LOCAL_ERROR(msg) logStream() << "[" << logTitle << "] " << "Error! " << msg << "\n"
	#define LOCAL_WARNING(msg) logStream() << "[" << logTitle << "] " << "Warning: " << msg << "\n"
	#define LOCAL_INFO(msg) logStream() << "[" << logTitle << "] " << msg << "\n"
	#define LOCAL_VERBOSE(msg) { if (verboseLevel) { logStream() << "[" << logTitle << "] " << "[V] " << msg << "\n"; } };
	#ifdef LOG_LEVEL_VERBOSE
		#define LOG_VERBOSE(msg) std::cout << "[V] " << msg << "\n"
	#else
		#define LOG_VERBOSE(msg)
	#endif
	#define LOG_PREFIX(prefix) const auto prefixInitializer = PrefixInitializer(prefix)
	#define LOG_POSTFIX(postfix) const auto postfixInitializer = PostfixInitializer(postfix)
	#define LOG(msg) logStream() << logPrefix << msg << logPostfix
	#define LOG_DEBUG(msg) logStream() << logPrefix << "[D] " << msg << logPostfix
	#define LOG_ERROR(msg) logStream() << logPrefix << "Error! " << msg << (logPostfix.empty() ? "\n" : logPostfix)
	#define LOG_WARNING(msg) logStream() << logPrefix << "Warning: " << msg << (logPostfix.empty() ? "\n" : logPostfix)
	#define LOG_INFO(msg) logStream() << logPrefix << msg << (logPostfix.empty() ? "\n" : logPostfix)
#else
	#define LOG_PREFIX(prefix)
	#define LOG_POSTFIX(postfix)
	#define LOG_TITLE(title)
	#define LOG_STREAM(s)
	#define SET_LOG_VERBOSE(boolVal)
	#define SET_LOG_DEBUG(boolVal)
	#define LOCAL_LOG(msg)
	#define LOCAL_DEBUG(msg)
	#define LOCAL_WARNING(msg)
	#define LOCAL_ERROR(msg)
	#define LOCAL_INFO(msg)
	#define LOCAL_VERBOSE(msg)
	#define LOG(msg)
	#define LOG_DEBUG(msg)
	#define LOG_WARNING(msg)
	#define LOG_ERROR(msg)
	#define LOG_INFO(msg)
	#define LOG_VERBOSE(msg)
#endif


