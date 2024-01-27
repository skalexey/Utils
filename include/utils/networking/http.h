#pragma once

#include <memory>
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
		template <typename UploaderT>
		// Keep "force" the last argument for safety
		std::shared_ptr<utils::networking::uploader_with_auth> upload_file_with_auth_async(const fs::path& local_path, const anp::tcp::endpoint_t& ep, const std::string& url_path, const std::string& user_name, const std::string& user_token, const utils::void_int_cb& on_result = {}, bool force = false)
		{
			using namespace anp;
			using namespace utils::networking;
			std::shared_ptr<UploaderT> u_base = std::make_shared<UploaderT>();
			static std::shared_ptr<utils::networking::uploader_with_auth> u = std::make_shared<utils::networking::uploader_with_auth>(user_name, user_token, u_base.get());
			query_t q;
			q.path = url_path;
			if (!force)
			{
				auto local_mtp = utils::file::modif_time(local_path.string() + ".last");
				long sec = std::chrono::duration_cast<std::chrono::seconds>(local_mtp.time_since_epoch()).count();
				q.add_value("modification_time", std::to_string(sec));
			}
			auto current_time = std::chrono::system_clock::now();
			auto sec = std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count();
			q.add_value("current_time", std::to_string(sec));
			u->upload_file_async(ep, local_path.string(), q, [=, capture = u](int code) {
				if (code == http_client_interface::erc::no_error)
					MSG("Uploaded '" << local_path.string() << "'");
				else
					LOG_ERROR("Error while uploading '" << local_path << "'");
				if (on_result)
					on_result(code);
			});
			return u;
		}

		// TODO: reuse upload_file_with_auth_async
		template <typename UploaderT>
		// Keep "force" the last argument for safety
		int upload_file_with_auth(const fs::path& local_path, const anp::tcp::endpoint_t& ep, const std::string& url_path, const std::string& user_name, const std::string& user_token, bool force = false)
		{
			auto u = upload_file_with_auth_async<UploaderT>(local_path, ep, url_path, user_name, user_token, nullptr, force);
			u->wait();
			return u->errcode();
		}
	}
}