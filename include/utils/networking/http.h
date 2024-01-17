#pragma once

#include <string>
#include <http/url_uploader.h>
#include <utils/networking/uploader_with_auth.h>
#include <utils/string_utils.h>
#include <utils/file_utils.h>
#include <utils/print_defs.h>

namespace utils
{
	namespace http
	{
		// TODO: reuse upload_file_with_auth_async
		template <typename UploaderT>
		int upload_file_with_auth(const fs::path &local_path, const anp::tcp::endpoint_t &ep, const std::string &url_path, const std::string &user_name, const std::string &user_token)
		{
			using namespace anp;
			using namespace utils::networking;
			std::shared_ptr<UploaderT> u_base = std::make_shared<UploaderT>();
			utils::networking::uploader_with_auth u(user_name, user_token, u_base.get());
			query_t q;
			q.path = url_path;
			auto r = u.upload_file(ep, local_path.string(), q);
			if (r == http_client_base::erc::no_error)
				MSG("Uploaded '" << local_path.string() << "'");
			else
				LOG_ERROR("Error while uploading '" << local_path << "'");
			return r;
		}

		template <typename UploaderT>
		void upload_file_with_auth_async(const fs::path &local_path, const anp::tcp::endpoint_t &ep, const std::string &url_path, const std::string &user_name, const std::string &user_token, const utils::void_int_cb &on_result)
		{
			using namespace anp;
			using namespace utils::networking;
			std::shared_ptr<UploaderT> u_base = std::make_shared<UploaderT>();
			static std::shared_ptr<utils::networking::uploader_with_auth> u = std::make_shared<utils::networking::uploader_with_auth>(user_name, user_token, u_base.get());
			query_t q;
			q.path = url_path;
			auto local_mtp = utils::file::modif_time(local_path);
			long sec = std::chrono::duration_cast<std::chrono::seconds>(local_mtp.time_since_epoch()).count();
			q.add_value("modification_time", std::to_string(sec));
			u->upload_file_async(ep, local_path.string(), q, [=, capture = u](int code) {
				if (code == http_client_base::erc::no_error)
					MSG("Uploaded '" << local_path.string() << "'");
				else
					LOG_ERROR("Error while uploading '" << local_path << "'");
				on_result(code);
			});
		}
	}
}