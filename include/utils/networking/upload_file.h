#pragma once

#include <string>
#include <utils/filesystem.h>
#include <utils/networking/http.h>

namespace utils
{
	namespace http
	{
		// Keep force the last argument for safety
		int upload_file(const fs::path& local_path, const anp::tcp::endpoint_t& ep, const std::string& url_path, bool force = false)
		{
			return upload_file_with_auth<anp::http::url_uploader>(local_path, ep, url_path, get_user_name(), get_user_token(), force);
		}
		
		// Keep force the last argument for safety
		void upload_file_async(const fs::path& local_path, const anp::tcp::endpoint_t& ep, const std::string& url_path, const utils::void_int_cb& on_result, bool force = false)
		{
			upload_file_with_auth_async<anp::http::url_uploader>(local_path, ep, url_path, get_user_name(), get_user_token(), on_result, force);
		}
	}
}