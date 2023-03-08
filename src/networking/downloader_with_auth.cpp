// downloader_with_auth.cpp

#include <utils/networking/downloader_with_auth.h>
#include <utils/networking/query_auth.h>

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
			return base::download_file(
				ep
				, q
				, target_path
				, std::bind(&downloader_with_auth::on_response, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, on_response)
			);
		}

		void downloader_with_auth::download_file_async(
			const endpoint_t& ep
			, const result_cb& on_result
			, const query_t& query
			, const fs::path& target_path
			, const http_response_cb& on_response
		)
		{
			query_auth q(query, m_user_name, m_token);
			base::download_file_async(
				ep
				, on_result
				, q
				, target_path
				, std::bind(&downloader_with_auth::on_response, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, on_response)
			);
		}

		bool downloader_with_auth::on_response(
			const headers_t& headers
			, const char* data
			, std::size_t sz
			, int status
			, const http_response_cb& cb
		)
		{
			std::string_view s(data, sz);
			if (s.find("Auth error") != std::string::npos)
			{
				notify(erc::auth_error);
				return false;
			}
			if (cb)
				if (!cb(headers, data, sz, status))
					return false;
			return true;
		}
	}
}