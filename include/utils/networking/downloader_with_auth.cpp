// downloader_with_auth.cpp

#include "downloader_with_auth.h"
#include "query_auth.h"

namespace utils
{
	namespace networking
	{
		using namespace anp;

		int downloader_with_auth::download_file(
			const endpoint_t& ep
			, const query_t& query
			, const fs::path& target_path
			, const http_response_cb& on_response
		)
		{
			query_auth q(query, m_user_name, m_token);
			return base::download_file(ep, q, target_path, [=, this] (
				const headers_t& headers
				, const char* data
				, std::size_t sz
				, int status
			) -> bool {
				std::string_view s(data, data + sz);
				if (s.find("Auth error") != std::string::npos)
				{
					notify(erc::auth_error);
					return false;
				}
				if (on_response)
					if (!on_response(headers, data, sz, status))
						return false;
				return true;
			});
		}
	}
}