#pragma once

#include <string>
#include <utils/filesystem.h>
#include <utils/networking/http.h>

namespace utils
{
	namespace http
	{
		int upload_file(const fs::path& local_path, const anp::tcp::endpoint_t& ep, const std::string& url_path)
		{
			return upload_file_with_auth<anp::http::url_uploader>(local_path, ep, url_path, get_user_name(), get_user_token());
		}
	}
}