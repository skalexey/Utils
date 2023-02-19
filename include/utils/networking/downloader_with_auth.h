﻿// downloader_with_auth.h

#pragma once

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
			enum erc : int
			{
				auth_error = downloader::erc::count * 100,
				end
			};
			downloader_with_auth(
				const std::string& user_name
				, const std::string& token
				, downloader_interface* object
			) : base(object)
				, m_user_name(user_name)
				, m_token(token)
			{}

			int download_file(
				const endpoint_t& ep
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_response_cb& on_response = {}
			) override;

		private:
			std::string m_token;
			std::string m_user_name;
		};
	}
}