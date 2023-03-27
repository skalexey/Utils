#pragma once

#include <string>
#include <utils/networking/downloader_with_auth.h>
#include <utils/networking/downloader_with_version_control.h>
#include <utils/networking/upload_file.h>
#include <utils/string_utils.h>
#include <utils/io_utils.h>
#include <utils/filesystem.h>
#include <utils/print_defs.h>
#include <utils/common.h>

extern void ask_user(
	const std::string& question
	, const utils::void_bool_cb& on_answer
	, const char* yes_btn_text
	, const char* no_btn_text
);

namespace
{
	using resources_list = std::vector<std::pair<std::string, fs::path>>;
	// TODO: support parallel download
	void download(
		utils::networking::downloader_with_version_control_ptr d
		, const anp::endpoint_t& ep
		, const std::string& url_path_download
		, const std::string& url_path_upload
		, const std::string& remote_path
		, const fs::path& local_path
		, const utils::void_int_cb& download_cb
	) {
		using namespace utils::networking;
		MSG("Download remote version of resource '" << local_path.filename() << "'...");
		query_t q;
		q.path = url_path_download;
		q.values.add("p", remote_path);
		std::string s;

		auto upload = [=]() {
			auto retcode = upload_file(local_path, ep, url_path_upload);
			// TODO: process error here
			d->update_last_version();
			download_cb(retcode);
		};

		d->download_file_async(ep, [=](int code) {
			auto ask_for_replace = [=]() {
				ask_user("Replace with the downloaded version?", [=](bool yes) {
					if (yes)
						download_cb(d->replace_with_download());
					else
						download_cb(0);
				});
			};
			if (code == downloader_with_version_control::erc::uncommitted_changes)
			{
				ask_user(
					STR("You have changes in '" << local_path << "'.\nWould you like to upload your file to the remote?")
					, [=](bool yes)
					{
						if (yes)
							upload();
						else
							ask_for_replace();
					}
				);
			}
			else if (code == downloader_with_version_control::erc::uncommitted_old_changes)
			{
				ask_user(
					STR("You have changes in '" << local_path << ", but there is a newer version available'.\nReplace your file with the downloaded version?")
					, [=](bool yes)
					{
						if (yes)
							download_cb(d->replace_with_download());
						else
							ask_user("Upload your file to the remote?", [=](bool yes) {
								if (yes)
									upload();
							});
					}
				);
			}
			else if (code == downloader::erc::parse_date_error)
				ask_for_replace();
			else if (code != http_client::erc::no_error)
			{
				LOG_ERROR("Error while downloading resource '" << remote_path << "'" << " to '" << local_path << "': " << code);
				download_cb(code);
			}
			else
			{
				if (d->is_file_updated())
					MSG("Resource updated from the remote: '" << local_path.string() << "'");
				else
					MSG("Local resource is up to date: '" << local_path.string() << "'");
				download_cb(0);
			}
		}, q, local_path); // != http_client::erc::no_error
	};

	void download_item_recursively(
		utils::networking::downloader_with_version_control_ptr d
		, const anp::endpoint_t& ep
		, const std::string& url_path_download
		, const std::string& url_path_upload
		, const resources_list& resources
		, int i
		, const utils::void_int_cb& on_result
	)
	{
		if (resources.empty())
		{
			on_result(10);
			return;
		}
		auto& r = resources[i];
		download(d, ep, url_path_download, url_path_upload, r.first, r.second, [=](int code) {
			if (code != 0)
				on_result(i + 1);
			else
			{
				if (i + 1 < resources.size())
					download_item_recursively(d, ep, url_path_download, url_path_upload, resources, i + 1, on_result);
				else
					on_result(0);
			}
		});
	};
}
namespace utils
{
	namespace networking
	{
		using resources_list = ::resources_list;
		void sync_resources(
			anp::endpoint_t& ep
			, const std::string& url_path_download
			, const std::string& url_path_upload
			, const resources_list& resources
			, const utils::void_int_cb& on_result = {}
		)
		{
			using namespace anp;
			using namespace utils::networking;
			downloader_ptr downloader_base = std::make_shared<downloader>();
			downloader_with_auth_ptr da = std::make_shared<downloader_with_auth>(get_user_name(), get_user_token(), downloader_base.get());
			downloader_with_version_control_ptr d = std::make_shared<downloader_with_version_control>(da.get());
			download_item_recursively(d, ep, url_path_download, url_path_upload, resources, 0, on_result);
		}
	}
}