// downloader_with_version_control.cpp

#include <cassert>
#include <utils/file_utils.h>
#include <utils/datetime.h>
#include <utils/networking/downloader_with_version_control.h>
#include <utils/networking/query_auth.h>
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
			const tcp::endpoint_t& ep
			, const query_t& query
			, const fs::path& target_path
			, const http_response_cb& on_response
		)
		{
			return base::download_file(
				ep
				, query
				, target_path
				, std::bind(&downloader_with_version_control::on_response, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, on_response)
			);
		}

		void downloader_with_version_control::download_file_async(
			const tcp::endpoint_t& ep
			, const result_cb& on_result
			, const query_t& query
			, const fs::path& target_path
			, const http_response_cb& on_response
		)
		{
			base::download_file_async(
				ep
				, on_result
				, query
				, target_path
				, std::bind(&downloader_with_version_control::on_response, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, on_response)
			);
		}

		fs::path downloader_with_version_control::get_last_version_path() const
		{
			return m_target.string() + ".last";
		}

		void downloader_with_version_control::on_before_download(const fs::path& target_path)
		{
			m_target = target_path;
			m_last_version_modified = utils::file::exists(get_last_version_path()) 
				&& !utils::file::same(m_target, get_last_version_path());
		}

		bool downloader_with_version_control::on_response(
			const http::headers_t& headers
			, const char* data
			, std::size_t sz
			, int status
			, const http_response_cb& cb
		)
		{
			// TODO: return bool on notify*() calls
			if (cb)
				if (!cb(headers, data, sz, status))
					return false;

			std::string_view s(data, sz);
			std::chrono::system_clock::time_point remote_mtp, local_mtp;
			local_mtp = utils::file::modif_time(m_target);

			LOG_DEBUG("	Init local file modification time: " << utils::time_to_string(local_mtp));

			auto& modification_time = headers.get("Modification-Time");
			if (!modification_time.empty())
			{
				try
				{
					remote_mtp = utils::parse_datetime_http(modification_time);
#if !defined(__APPLE__) && !defined(__GNUC__) && __cplusplus >= 202002L
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
			#ifdef LOG_LEVEL_DEBUG
				auto local_time = utils::time_to_string(local_mtp);
				auto remote_time = utils::time_to_string(remote_mtp);
			#endif
				LOG_DEBUG("local_time: " << local_time);
				LOG_DEBUG("remote_time: " << remote_time);
				if (local_mtp < remote_mtp)
					LOG_DEBUG("m_f_tp < remote_mtp (" << utils::time_to_string(local_mtp) << ", " << utils::time_to_string(remote_mtp) << ")");
				else // The local file is newer;
					LOG_DEBUG("The local file is newer");

				if (content_differs)
					notify(m_last_version_modified ? 
						local_mtp < remote_mtp ? erc::uncommitted_old_changes : erc::uncommitted_changes
						: replace_with_download()
					);
				else
				{
					LOG_DEBUG("Files contents are the same");
					notify(http_client_base::erc::no_error);
				}
			}
			return true;
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

		int downloader_with_version_control::replace_with_download()
		{
			if (!backup_local_file())
				return erc::backup_error;
			if (utils::file::move(get_file_path(), m_target) == 0)
			{
				if (utils::file::copy(m_target, get_last_version_path()) != 0)
					return erc::update_last_version_error;
				m_is_file_updated = true;
				LOG_VERBOSE("Download file successfully stored into the local file's path '" << m_target << "'");
				return http_client_base::erc::no_error;
			}
			LOG_ERROR("Error while placing the downloaded file '" << get_file_path() << "' into the target path '" << m_target << "'");
			return erc::store_download_error;
		}

		void downloader_with_version_control::on_notify(int ec)
		{
			if (errcode() == http_client_base::erc::no_error)
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

		bool downloader_with_version_control::update_last_version()
		{
			if (!m_target.empty())
				return utils::file::copy(m_target, get_last_version_path()) == 0;
			return false;
		}
	}
}