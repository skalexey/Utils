// downloader_with_auth.h

#pragma once

#include <memory>
#include <string>
#include <utils/filesystem.h>
#include <http/downloader_decorator.h>
#include <http/downloader.h>
#include "query_auth.h"

namespace utils
{
	namespace networking
	{
		using namespace anp;

		class downloader_with_auth : public downloader_decorator
		{
			using base = downloader_decorator;

		public:
			downloader_with_auth(
				const std::string& user_name
				, const std::string& token
				, downloader_interface* object
			) : base(object)
				, m_user_name(user_name)
				, m_token(token)
			{}

			int download_file(
				const tcp::endpoint_t& ep
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_response_cb& on_response = {}
			) override;

			void download_file_async(
				const tcp::endpoint_t& ep
				, const result_cb& on_result = {}
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_response_cb& on_response = {}
			) override;

			bool on_response(
				const http::headers_t& headers
				, const char* data
				, std::size_t sz
				, int status
				, const http_response_cb& cb = {}
			);

		private:
			std::string m_token;
			std::string m_user_name;
		};
		using downloader_with_auth_ptr = std::shared_ptr<downloader_with_auth>;
	}
}