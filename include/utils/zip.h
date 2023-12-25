#pragma once

#include <vector>
#include <string>
#include <stddef.h>
#include <type_traits>

namespace utils
{
	namespace zip
	{
		using byte_t = unsigned char;
		using data_t = std::vector<byte_t>;
		int compress_text(const std::string& data, data_t& out);
		int compress_binary(const data_t& data, data_t& out);

		inline int compress(const std::string& data, data_t& out)
		{
			return compress_text(data, out);
		}

		inline int compress(const data_t& data, data_t& out)
		{
			return compress_binary(data, out);
		}

		int decompress_binary(const data_t& data, data_t& out);
		int decompress_text(const data_t& data, std::string& out);
		template <typename T>
		int decompress(const data_t& data, T& out)
		{
			if (std::is_same<T, std::string>::value)
				return decompress_text(data, out);
			return decompress_binary(data, out);
		}
	}
}
