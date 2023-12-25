#pragma once

#include <string>
#include <http/url_uploader.h>
#include <utils/networking/uploader_with_auth.h>
#include <utils/string_utils.h>
#include <utils/filesystem.h>
#include <utils/print_defs.h>

namespace utils
{
	namespace http
	{
		template <typename UploaderT>
		int upload_file_with_auth(const fs::path &local_path, const anp::tcp::endpoint_t &ep, const std::string &url_path, const std::string &user_name, const std::string &user_token)
		{
			using namespace anp;
			using namespace utils::networking;
			UploaderT u_base;
			utils::networking::uploader_with_auth u(user_name, user_token, &u_base);
			query_t q;
			q.path = url_path;
			auto r = u.upload_file(ep, local_path.string(), q);
			if (r == http_client_base::erc::no_error)
				MSG("Uploaded '" << local_path.string() << "'");
			else
				LOG_ERROR("Error while uploading '" << local_path << "'");
			return r;
		}
	}
}