// downloader_with_version_control.h

#pragma once

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
				no_date,
				parse_date_error,
				no_file,
				file_error,
				backup_error,
				store_download_error,
				auth_error
			};
			downloader_with_version_control(downloader_interface* object) : base(object) {}

			int download_file(
				const endpoint_t& ep
				, const query_t& query = {}
				, const fs::path& target_path = {}
				, const http_client::http_response_cb& on_response = {}
			) override;

			bool is_file_updated();
			// TODO: see how to return it to the protected
			bool replace_with_download();

		private:
			bool backup_local_file();
			bool restore_from_backup();
			bool remove_backup();

		protected:
			void on_notify(int ec) override;
			void on_reset() override;

		private:
			fs::path m_target;
			fs::path m_backup;
			bool m_is_file_updated = false;
		};
	}
}