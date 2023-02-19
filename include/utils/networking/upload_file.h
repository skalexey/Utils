#pragma once

#include <string>
#include <http/uploader.h>
#include <utils/networking/uploader_with_auth.h>
#include <utils/string_utils.h>
#include <utils/filesystem.h>
#include <utils/print_defs.h>
#include <utils/dmb/auth.h>

bool upload_file(const fs::path& local_path, const anp::endpoint_t& ep)
{
	using namespace anp;
	using namespace utils::networking;
    uploader u_base;
	uploader_with_auth u(get_user_name(), get_user_token(), &u_base); 
	query_t q;
	q.path = "/v/h.php";
	if (u.upload_file(ep, local_path.string(), q) == http_client::erc::no_error)
	{
		MSG("Uploaded '" << local_path.string() << "'");
		return true;
	}
	else
		LOG_ERROR("Error while uploading '" << local_path << "'");
	return false;
}