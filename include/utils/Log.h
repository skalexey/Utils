#pragma once

#ifdef LOG_ON
	#include <iostream>
	#include <utils/log_stream.h>
	#include <utils/metaprogramming.h>
	
	namespace
	{
		using log_stream = std::conditional<is_complete<utils::user_log_stream>::value, utils::user_log_stream, utils::default_log_stream>::type;

		std::string logPrefix; // Prefix for LOG_* commands. Is set through LOG_PREFIX function
		std::string logPostfix = "\n"; // Postfix for LOG_* commands. Is set through LOG_POSTFIX function

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
	#define LOCAL_LOG(msg) log_stream() << logPrefix << msg << "\n"
	#ifdef LOG_LEVEL_DEBUG
		#define LOG_DEBUG(msg) log_stream() << logPrefix << "[D] " << msg << logPostfix
		#define LOCAL_DEBUG(msg) { if (debugLevel) { LOG_DEBUG(msg); } };
		#define SET_LOCAL_LOG_DEBUG(boolVal) namespace { bool debugLevel = boolVal; }
	#else
		#define LOG_DEBUG(msg)
		#define LOCAL_DEBUG(msg)
		#define SET_LOCAL_LOG_DEBUG(boolVal)
	#endif
	#define LOCAL_ERROR(msg) log_stream() << logPrefix << "Error! " << msg << "\n"
	#define LOCAL_WARNING(msg) log_stream() << logPrefix << "Warning: " << msg << "\n"
	#define LOCAL_INFO(msg) log_stream() << logPrefix << msg << "\n"
	#ifdef LOG_LEVEL_VERBOSE
        #define LOG_VERBOSE(msg) log_stream() << logPrefix << "[V] " << msg << "\n"
		#define LOCAL_VERBOSE(msg) { if (verboseLevel) { LOG_VERBOSE(msg); } };
		#define SET_LOCAL_LOG_VERBOSE(boolVal) namespace { bool verboseLevel = boolVal;}
	#else
		#define LOG_VERBOSE(msg)
		#define LOCAL_VERBOSE(msg)
		#define SET_LOCAL_LOG_VERBOSE(boolVal)
	#endif
	#define LOG_PREFIX(prefix) const auto prefixInitializer = PrefixInitializer(prefix);
	#define LOG_POSTFIX(postfix) const auto postfixInitializer = PostfixInitializer(postfix);
	#define LOG_TITLE(prefix) const auto prefixInitializer = PrefixInitializer("["#prefix"]: ");
	#define LOG(msg) log_stream() << logPrefix << msg << logPostfix
	#define LOG_ERROR(msg) log_stream() << logPrefix << "Error! " << msg << (logPostfix.empty() ? "\n" : logPostfix)
	#define LOG_WARNING(msg) log_stream() << logPrefix << "Warning: " << msg << (logPostfix.empty() ? "\n" : logPostfix)
	#define LOG_INFO(msg) log_stream() << logPrefix << msg << (logPostfix.empty() ? "\n" : logPostfix)
	
	// Print and invoke an expression
	#define LOG_EXPR(ex) LOG(#ex); ex
	#define LOG_EXPR_LOG(ex, msg) log_stream() << logPrefix << #ex << msg << logPostfix; ex
	#define LOG_EXPR_VAL(ex) LOG(ex)
	#define LOG_EXPR_AND_VAL(ex) LOG(#ex << ": " << ex)
	#define LOG_EXPR_AND_VAL_LOG(ex, msg) LOG(#ex << ": " << ex << msg)
#else
	#define LOG_PREFIX(prefix)
	#define LOG_POSTFIX(postfix)
	#define LOG_TITLE(title)
	#define SET_LOCAL_LOG_VERBOSE(boolVal)
	#define SET_LOCAL_LOG_DEBUG(boolVal)
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
	#define LOG_EXPR(ex)
	#define LOG_EXPR_LOG(ex)
	#define LOG_EXPR_VAL(ex)
	#define LOG_EXPR_AND_VAL(ex)
	#define LOG_EXPR_AND_VAL_LOG(ex, msg)
#endif


