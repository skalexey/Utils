// uploader_with_auth.h

#pragma once

#include <string>
#include <utils/filesystem.h>
#include <http/uploader_decorator.h>

namespace utils
{
	namespace networking
	{
		using namespace anp;
		class uploader_with_auth : public http::uploader_decorator
		{
			using base = http::uploader_decorator;

		public:
			uploader_with_auth(
				const std::string& user_name
				, const std::string& token
				, uploader_interface* object
			) : base(object)
				, m_user_name(user_name)
				, m_token(token)
			{}

			void upload_file_async(
				const tcp::endpoint_t& ep,
				const fs::path& target_path,
				const query_t& query,
				const utils::void_int_cb& cb = nullptr
			) override;

		private:
			std::string m_token;
			std::string m_user_name;
		};
	}
}