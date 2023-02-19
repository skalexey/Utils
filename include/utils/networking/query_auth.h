#pragma once

#include <http/query.h>

namespace utils
{
	namespace networking
	{
		using namespace anp;
		class query_auth : public query_t
		{
			void init(const std::string& user_name, const std::string& token)
			{
				values.add("u", user_name);
				values.add("t", token);
			}
		public:
			query_auth(const query_t& q) : query_t(q) {}
			query_auth(const std::string& user_name, const std::string& token)
			{
				init(user_name, token);
			}
			query_auth(const query_t& q, const std::string& user_name, const std::string& token) : query_auth(q)
			{
				init(user_name, token);
			}
		};
	}
}
