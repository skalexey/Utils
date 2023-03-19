#pragma once

#include <string>
#include <sstream>
#include <utils/networking/downloader_with_auth.h>
#include <utils/networking/downloader_with_version_control.h>
#include <utils/networking/upload_file.h>
#include <utils/string_utils.h>
#include <utils/io_utils.h>
#include <utils/filesystem.h>
#include <utils/print_defs.h>
#include <utils/dmb/auth.h>


namespace utils
{
	namespace networking
	{
		using resources_list = std::vector<std::pair<std::string, fs::path>>;

		int sync_resources(
			anp::endpoint_t& ep
			, const std::string& url_path_download
			, const std::string& url_path_upload
			, const resources_list& resources
			, const anp::void_int_cb& on_result = {}
		)
		{
			using namespace anp;
			using namespace utils::networking;
			downloader downloader_base;
			downloader_with_auth da(get_user_name(), get_user_token(), &downloader_base);
			downloader_with_version_control d(&da);
			auto download = [&](const std::string& remote_path, const fs::path& local_path) -> bool {
				MSG("Download remote version of resource '" << local_path.filename() << "'...");
				query_t q;
				q.path = url_path_download;
				q.values.add("p", remote_path);
				if (d.download_file(ep, q, local_path, nullptr ) != http_client::erc::no_error)
				{
					if (d.errcode() == downloader_with_version_control::erc::uncommitted_changes)
					{
						std::stringstream ss;
						ss << "You have changes in '" << local_path << "'.\nWould you like to upload your file to the remote?";
						try
						{
							if (utils::input::ask_user(
								ss.str()))
							{
								if (!upload_file(local_path, ep, url_path_upload))
									return false;
							}
							else
							{
								if (utils::input::ask_user("Replace with the downloaded version?"))
									d.replace_with_download();
							}
						}
						catch (std::string s)
						{
							MSG("Emergency exit (" << s << ")");
							return false;
						}
						return true;
					}
					else if (d.errcode() == downloader::erc::parse_date_error)
						if (utils::input::ask_user("Replace with the downloaded version?"))
							d.replace_with_download();
					LOG_ERROR("Error while downloading resource '" << remote_path << "'" << " to '" << local_path << "': " << d.errcode());
					return false;
				}
				if (d.is_file_updated())
					MSG("Resource updated from the remote: '" << local_path.string() << "'");
				else
					MSG("Local resource is up to date: '" << local_path.string() << "'");
				return true;
			};

			for (int i = 0; i < resources.size(); i++)
			{
				auto& r = resources[i];
				if (!download(r.first, r.second))
				{
					if (on_error)
						on_error(d.errcode());
					MSG("");
					return i + 1;
				}
				MSG("");
			}
			if (on_complete)
				on_complete();
			return 0;
		}
	}
}