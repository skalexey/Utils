// downloader_with_version_control.cpp

#include <cassert>
#include <utils/file_utils.h>
#include <utils/datetime.h>
#include "downloader_with_version_control.h"
#include "query_auth.h"

#include <utils/log.h>

LOG_PREFIX("[downloader_with_version_control]: ");
LOG_POSTFIX("\n");
SET_LOG_DEBUG(true);

namespace utils
{
	namespace networking
	{
		using namespace anp;

		int downloader_with_version_control::download_file(
			const endpoint_t& ep
			, const query_t& query
			, const fs::path& target_path
			, const http_response_cb& on_response
		)
		{
			m_target = target_path;

			return base::download_file(ep, query, target_path, [=, this](
				const headers_t& headers
				, const char* data
				, std::size_t sz
				, int status
				) -> bool {
					// TODO: return bool on notify*() calls
					if (on_response)
						if (!on_response(headers, data, sz, status))
							return false;

					std::string_view s(data, data + sz);
					std::chrono::system_clock::time_point remote_mtp, local_mtp;
					local_mtp = utils::file::modif_time(m_target);

					LOG_DEBUG("	Init local file modification time: " << utils::time_to_string(local_mtp));

					auto& modification_time = headers.get("Modification-Time");
					if (!modification_time.empty())
					{
						try
						{
							remote_mtp = utils::parse_datetime_http(modification_time);
			#if !defined(__APPLE__) && !defined(__GNUC__)
							const std::chrono::zoned_time local("Australia/Sydney", remote_mtp);
							LOG_VERBOSE("Received modification time: " << local);
			#endif
							// tz lib can be used to support C++11/14/17,
							// but it is already integrated into C++20.
							// Info:
							// https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
							// https://howardhinnant.github.io/date/tz.html
							// https://github.com/HowardHinnant/date
							// date lib info:
							// https://howardhinnant.github.io/date/date.html
							// , %d %B %4C %H:%M:%S GMT
							LOG_DEBUG("Parsed date: " << utils::time_to_string(remote_mtp));
							LOG_DEBUG("Local file date: " << utils::time_to_string(local_mtp));
						}
						catch (...)
						{
							LOG_ERROR("Can't parse file modification date '" << modification_time << "'");
							notify(erc::parse_date_error);
							return true;
						}
					}
					else
					{
						LOG_ERROR("No modification date received from the server");
						notify(erc::no_file);
						return true;
					}

					if (utils::file::exists(m_target) && local_mtp == decltype(local_mtp)())
					{
						notify(erc::file_error);
					}
					else
					{
						assert(utils::file::exists(m_target));
						// Check if files contents differ
						bool content_differs = false;
						if (!utils::file::same(get_file_path(), m_target))
							content_differs = true;

						// Check the modification time
						LOG_DEBUG("Check the modification time");
						if (local_mtp < remote_mtp)
						{
							LOG_DEBUG("m_f_tp < remote_mtp (" << utils::time_to_string(local_mtp) << ", " << utils::time_to_string(remote_mtp) << ")");
							// Overwrite with the download
							if (content_differs)
							{
								if (!backup_local_file())
									notify(erc::backup_error);
								else
								{
									if (replace_with_download())
										notify(http_client::erc::no_error);
									else
										notify(erc::store_download_error);
								}
							}
							else
								notify(http_client::erc::no_error);
						}
						else
						{
							// The local file is newer;
							LOG_DEBUG("The local file is newer");
							if (content_differs)
								notify(erc::uncommitted_changes);
							else
							{
								LOG_DEBUG("Files contents are the same");
								notify(http_client::erc::no_error);
							}
						}
					}
					return true;
				}
			);
		}

		bool downloader_with_version_control::is_file_updated()
		{
			return m_is_file_updated;
		}

		bool downloader_with_version_control::backup_local_file()
		{
			assert(!m_target.empty());
			fs::path bac = m_target.parent_path() / fs::path(m_target.filename().string() + ".bac");
			if (utils::file::copy(m_target, bac) == 0)
			{
				LOG_VERBOSE("Backup of the local file created in '" << bac << "'");
				m_backup = bac;
				return true;
			}
			LOG_ERROR("Error while backuping the local file '" << m_target << "' into '" << bac << "'");
			return false;
		}

		bool downloader_with_version_control::replace_with_download()
		{
			if (utils::file::move(get_file_path(), m_target) == 0)
			{
				m_is_file_updated = true;
				LOG_VERBOSE("Download file successfully stored into the local file's path '" << m_target << "'");
				return true;
			}
			LOG_ERROR("Error while placing the downloaded file '" << get_file_path() << "' into the target path '" << m_target << "'");
			return false;
		}

		void downloader_with_version_control::on_notify(int ec)
		{
			if (errcode() == http_client::erc::no_error)
				remove_backup();
			else
				restore_from_backup();
		}

		void downloader_with_version_control::on_reset()
		{
			m_target.clear();
			m_backup.clear();
			LOG_DEBUG("Reset remote file modification time");
			m_is_file_updated = false;
		}

		bool downloader_with_version_control::restore_from_backup()
		{
			if (!m_backup.empty())
				if (!m_target.empty())
					return utils::file::move(m_backup, m_target) == 0;
			return false;
		}

		bool downloader_with_version_control::remove_backup()
		{
			if (!m_backup.empty())
				return utils::file::remove(m_backup);
			return false;
		}
	}
}