#pragma once

#include <string>
#include <utils/networking/downloader_with_auth.h>
#include <utils/networking/downloader_with_version_control.h>
#include <utils/networking/upload_file.h>
#include <utils/string_utils.h>
#include <utils/filesystem.h>
#include <utils/print_defs.h>
#include <utils/datetime.h>
#include <utils/common.h>
#include <utils/extern/user_input.h>

namespace utils
{
	namespace networking
	{
		using resource_t = std::pair<std::string, fs::path>;
		using resources_list = std::vector<resource_t>;
	}
}

extern int upload_changes(
	const utils::void_int_cb& on_result
	, const utils::networking::resource_t& resource
	, int resource_index
	, bool async = true
	, bool force = false
);

namespace
{
	// Download resource scenario including asking a user to send or overwrite their local changes.
	// TODO: support parallel download
	int download(
		utils::networking::downloader_with_version_control_ptr d
		, const anp::tcp::endpoint_t& ep
		, const std::string& url_path_download
		, const utils::networking::resource_t& resource
		, int resource_index
		, const utils::void_int_cb& cb
		, bool async = true
	) {
		using namespace utils::networking;
		auto& remote_path = resource.first;
		auto& local_path = resource.second;
		MSG("Download remote version of resource '" << local_path.filename() << "'...");
		query_t q;
		q.path = url_path_download;
		q.add_value("p", remote_path);
		std::string s;

		enum upload_options : int
		{
			none,
			force,
		};

		auto option_present = [=](int options, int option) {
			return (options & option) == option;
		};

		auto retcode = std::make_shared<int>(0);

		auto upload = [=](const resource_t& resource, int resource_index, int options = upload_options::none) {
				upload_changes([=](int code) {
					if (code == 0)
					{
						d->update_last_version();
						cb(code);
					}
					else
					{
						ask_user("Uploading error. Your last changes were not saved on the remote server. You still can work, but keep in mind, that your changes may get lost once you loose access to this device. Continue?", [=](bool yes) {
							cb(*retcode = yes ? 0 : -1);
						});
					}
				}
				, resource
				, resource_index
				, async
				, option_present(options, upload_options::force)
			);
		};

		auto on_download = [=, &resource](int code) {
			auto ask_for_replace = [=]() {
				ask_user("Replace with the downloaded version?", [=](bool yes) {
					if (yes)
						cb(*retcode = d->replace_with_download());
					else
						cb(0);
					});
			};
			if (code == http_client_interface::erc::uncommitted_changes)
			{
				ask_user(
					STR("You have changes in '" << local_path << "'.\nWould you like to upload your file to the remote?")
					, [=, &resource](bool yes)
					{
						if (yes)
							upload(resource, resource_index);
						else
							ask_for_replace();
					}
				);
			}
			else if (code == http_client_interface::erc::uncommitted_old_changes)
			{
				ask_user(
					STR("You have changes in '" << local_path << ", but there is a newer version available'.\nReplace your file with the downloaded version?")
					, [=, &resource](bool yes)
					{
						if (yes)
							cb(*retcode = d->replace_with_download());
						else
							ask_user("Upload your file to the remote?", [=](bool yes) {
								if (yes)
									upload(resource, resource_index);
							});
					}
				);
			}
			else if (code == http_client_interface::erc::no_file)
			{
				ask_user(
					STR("The remote server does not have your '" << remote_path << "' data stored. Would you like to upload your local changes to the remote?")
					, [=, &resource](bool yes)
					{
						if (yes)
							// Force option is used to avoid skipping upon the local changes check
							upload(resource, resource_index, upload_options::force);
						else
							cb(0);
					}
				);
			}
			else if (code == http_client_interface::erc::parse_date_error)
				ask_for_replace();
			else if (code != http_client_interface::erc::no_error)
			{
				LOG_ERROR("Error while downloading resource '" << remote_path << "'" << " to '" << local_path << "': " << code);
				cb(*retcode = code);
			}
			else
			{
				if (d->is_file_updated())
					MSG("Resource updated from the remote: '" << local_path.string() << "'");
				else
					MSG("Local resource is up to date: '" << local_path.string() << "'");
				cb(0);
			}
		};

		if (async)
			d->download_file_async(ep, on_download, q, local_path); // != http_client_interface::erc::no_error
		else
		{
			auto result = d->download_file(ep, q, local_path);
			on_download(result);	
		}
		
		return *retcode;
	};
}

namespace utils
{
	namespace networking
	{
		using processor_t = std::function<int(const resource_t&, int, const utils::int_int_cb&)>;

		int process_item_recursively(
			const processor_t& processor
			, const resources_list& resources
			, int i
			, const utils::void_int_cb& on_result = nullptr
		)
		{
			if (resources.empty())
			{
				if (on_result)
					on_result(10);
				return 10;
			}
			return processor(resources[i], i, [=, &resources](int code) {
				if (code != 0)
				{
					if (on_result)
						on_result(code);
				}
				else
				{
					if (i + 1 < resources.size())
						return process_item_recursively(processor, resources, i + 1, on_result);
					else
						if (on_result)
							on_result(0);
				}
				return code;
			});
		};
	
		// sync_resources()
		// Returns a value if async is false. Otherwise returns 0. The result code is passed to on_result callback anyway.
		int sync_resources(
			anp::tcp::endpoint_t& ep
			, const std::string& url_path_download
			, const std::string& url_path_upload
			, const resources_list& resources
			, const utils::void_int_cb& on_resource_processed = {}
			, bool async = true
		)
		{
			using namespace anp;
			using namespace utils::networking;
			downloader_ptr downloader_base = std::make_shared<downloader>();
			downloader_with_auth_ptr da = std::make_shared<downloader_with_auth>(get_user_name(), get_user_token(), downloader_base.get());
			downloader_with_version_control_ptr d = std::make_shared<downloader_with_version_control>(da.get());
			auto result = process_item_recursively(
				[=, &resources](const resource_t& resource, int resource_index, const utils::void_int_cb& on_process_result) {
					return download(d, ep, url_path_download, resource, resource_index, on_process_result, async);
				}
				, resources
				, 0
				, on_resource_processed
			);
			if (!async)
				return result;
			return 0;
		}

		int upload_changes(
			anp::tcp::endpoint_t& ep
			, const std::string& url_path_upload
			, const resource_t& r
			, int resource_index = 0
			, const utils::void_int_cb& on_result = {}
			, bool async = true
			, bool force = false // Keep it the last argument for safety
		)
		{
			if (utils::file::same(r.second, r.second.string() + ".last") && !force)
			{
				MSG("No changes in '" << r.second << "'");
				on_result(0);
				return 0;
			}
			auto upload_job = [=](int code) {
				if (code == 0)
				{
					MSG("Changes uploaded '" << r.second << "'");
					auto mtp = utils::file::modif_time(r.second);
					auto time_str = utils::time_to_string(mtp);
					MSG("Modification time before copy: " << time_str);
					auto last_version_fname = r.second.string() + ".last";
					utils::file::copy(r.second, last_version_fname, false, true);
					MSG("Copied '" << r.second << "' to '" << last_version_fname << "'");
					mtp = utils::file::modif_time(last_version_fname);
					time_str = utils::time_to_string(mtp);
					MSG("Modification time after copy: " << time_str);
				}
				else
					LOG_ERROR("Error while uploading (" << code << "): '" << r.second << "'");
				on_result(code);
			};
			if (async)
			{
				utils::http::upload_file_async(r.second, ep, url_path_upload, upload_job, force);
				return 0;
			}
			else
			{
				auto result = utils::http::upload_file(r.second, ep, url_path_upload, force);
				upload_job(result);
				return result;
			}
		}

		// Simple upload result processor
		void on_upload_changes(int code, const resource_t& resource, int resource_index, bool async = true, const utils::void_int_cb& processor = nullptr)
		{
			if (code != 0)
			{
				if (code == anp::http_client_interface::erc::newer_version_on_server)
				{
					ask_user(
						"Couldn't upload your changes because there is a newer version on the server. Would you still like to upload your version and overwrite the changes on the server?"
						, [=, &resource](bool yes) {
							if (yes)
								::upload_changes(processor, resource, resource_index, async, true);
							else
							{
								if (processor)
									processor(code);
							}
						}
					);
				}
				else
				{
					show_message("upload_changes() failed with error code: " + std::to_string(code));
				}
			}
			else
			{
				if (processor)
					processor(code);
			}
		}
	}
}