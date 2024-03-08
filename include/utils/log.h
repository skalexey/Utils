#pragma once

#ifdef LOG_ON
	#include <iostream>
	#include <utils/log_stream.h>
	#include <utils/metaprogramming.h>
	
	namespace
	{
		using log_stream = std::conditional<is_complete<utils::user_log_stream>::value, utils::user_log_stream, utils::default_log_stream>::type;

		std::string log_prefix; // Prefix for LOG_* commands. Is set through LOG_PREFIX function
		std::string log_postfix = "\n"; // Postfix for LOG_* commands. Is set through LOG_POSTFIX function

		enum log_level_t : int
		{
			log_level_error,
			log_level_warning,
			log_level_info,
			log_level_debug,
			log_level_verbose,
			log_level_all
		};
		struct prefix_initializer
		{
			prefix_initializer(const std::string& s) {
				log_prefix = s;
			}
		};

		struct postfix_initializer
		{
			postfix_initializer(const std::string& s) {
				log_postfix = s;
			}
		};
	}

	#define LOG_PREFIX(prefix) const auto _prefix_initializer_ = prefix_initializer(prefix);
	#define LOG_POSTFIX(postfix) const auto _postfix_initializer_ = postfix_initializer(postfix);
	#define LOG_TITLE(prefix) const auto _prefix_initializer_ = prefix_initializer("["prefix"]: ");

	#define LOUT(msg) log_stream() << msg
	#define LOG(msg) LOUT(log_prefix << msg << log_postfix)
	#define LOCAL_LOG(msg) { if (local_log_level >= 0) { LOG(msg); } }
	#define SET_LOCAL_LOG_LEVEL(level) namespace { int local_log_level = log_level_##level; }
	#ifdef LOG_LEVEL_DEBUG
		#define LOG_DEBUG(msg) LOG("[D] " << msg)
		#define LOCAL_DEBUG(msg) { if (local_log_level == log_level_debug) { LOG_DEBUG(msg); } };
	#else
		#define LOG_DEBUG(msg)
		#define LOCAL_DEBUG(msg)
	#endif
	#ifdef LOG_LEVEL_VERBOSE
		#define LOG_VERBOSE(msg) LOG("[V] " << msg)
		#define LOCAL_VERBOSE(msg) { if (local_log_level == log_level_verbose) { LOG_VERBOSE(msg); } };
	#else
		#define LOG_VERBOSE(msg)
		#define LOCAL_VERBOSE(msg)
	#endif
	#define LOG_ERROR(msg) LOG("[E] Error! " << msg)
	#define LOG_WARNING(msg) LOG("[W] Warning: " << msg)
	#define LOG_INFO(msg) LOG("[I] " << msg)
	#define LOCAL_ERROR(msg) { if (local_log_level == log_level_error) { LOG_ERROR(msg); } }
	#define LOCAL_WARNING(msg) { if (local_log_level == log_level_warning) { LOG_WARNING(msg); } }
	#define LOCAL_INFO(msg) { if (local_log_level == log_level_info) { LOG_INFO(msg); } }
	
	// Print and invoke an expression
	#define LOG_EXPR(ex) LOG(#ex); ex
	#define LOG_EXPR_LOG(ex, msg) LOG(#ex << msg); ex
	#define LOG_EXPR_VAL(ex) LOG(ex)
	#define LOG_EXPR_AND_VAL(ex) LOG(#ex << ": " << ex)
	#define LOG_EXPR_AND_VAL_LOG(ex, msg) LOG(#ex << ": " << ex << msg)
#else
	#define LOG_PREFIX(prefix) {}
	#define LOG_POSTFIX(postfix) {}
	#define LOG_TITLE(title) {}
	#define SET_LOCAL_LOG_LEVEL(level) {}
	#define LOCAL_LOG(msg) {}
	#define LOCAL_DEBUG(msg) {}
	#define LOCAL_WARNING(msg) {}
	#define LOCAL_ERROR(msg) {}
	#define LOCAL_INFO(msg) {}
	#define LOCAL_VERBOSE(msg) {}
	#define LOG(msg) {}
	#define LOG_DEBUG(msg) {}
	#define LOG_WARNING(msg) {}
	#define LOG_ERROR(msg) {}
	#define LOG_INFO(msg) {}
	#define LOG_VERBOSE(msg) {}
	#define LOG_EXPR(ex) {}
	#define LOG_EXPR_LOG(ex) {}
	#define LOG_EXPR_VAL(ex) {}
	#define LOG_EXPR_AND_VAL(ex) {}
	#define LOG_EXPR_AND_VAL_LOG(ex, msg) {}
#endif


