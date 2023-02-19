// uploader_with_auth.h

#pragma once

#include <string>
#include <utils/filesystem.h>
#include "uploader_decorator.h"

namespace utils
{
	namespace networking
	{
		using namespace anp;

		class uploader_with_auth : public uploader_decorator
		{
			using base = uploader_decorator;

		public:
			uploader_with_auth(
				const std::string& user_name
				, const std::string& token
				, uploader_interface* object
			) : base(object)
				, m_user_name(user_name)
				, m_token(token)
			{}

			int upload_file(
				const endpoint_t& ep,
				const fs::path& target_path,
				const query_t& query
			) override;

		private:
			std::string m_token;
			std::string m_user_name;
		};
	}
}