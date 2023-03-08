// uploader_with_auth.cpp

#include <utils/networking/uploader_with_auth.h>
#include <utils/networking/query_auth.h>

namespace utils
{
	namespace networking
	{
		using namespace anp;

		int uploader_with_auth::upload_file(
			const endpoint_t& ep,
			const fs::path& target_path,
			const query_t& query
		)
		{
			query_auth q(query, m_user_name, m_token);
			return base::upload_file(ep, target_path, q);
		}
	}
}