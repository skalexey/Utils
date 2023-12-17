// downloader_with_version_control.h

#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <utils/filesystem.h>
#include <http/downloader_decorator.h>
#include <http/downloader.h>

namespace utils
{
	namespace networking
	{
		using namespace anp;

		class downloader_with_version_control : public downloader_decorator
		{
			using base = downloader_decorator;

		public:
			enum erc : int
			{
				uncommitted_changes = downloader::erc::count * 200,
				uncommitted_old_changes,
				no_date,
				parse_date_error,
				no_file,
				file_error,
				backup_error,
				store_download_error,
				update_last_version_error,
				auth_error,
			};
			downloader_with_version_control(downloader_interface* object) : base(object) {}

			int download_file(
				const tcp::endpoint_t& ep
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_response_cb& on_response = {}
			) override;

			void download_file_async(
				const tcp::endpoint_t& ep
				, const result_cb& on_result = {}
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_response_cb& on_response = {}
			) override;

			bool is_file_updated();
			// TODO: see how to return it to the protected
			int replace_with_download();

			bool on_response(
				const http::headers_t& headers
				, const char* data
				, std::size_t sz
				, int status
				, const http_response_cb& cb = {}
			);

		bool update_last_version();

		private:
			bool backup_local_file();
			bool restore_from_backup();
			bool remove_backup();
			fs::path get_last_version_path() const;

		protected:
			void on_notify(int ec) override;
			void on_reset() override;
			void on_before_download(const fs::path& target_path) override;

		private:
			fs::path m_target;
			fs::path m_backup;
			bool m_is_file_updated = false;
			bool m_last_version_modified = false;
		};
		using downloader_with_version_control_ptr = std::shared_ptr<downloader_with_version_control>;
	}
}