#include <utils/zip.h>
#include <zlib.h>
#include <utils/Log.h>

LOG_TITLE("utils::zip");

namespace
{
	// void compress_data(void* in_data, std::size_t in_data_size, utils::zip::data_t& out_data)
	// {
	// 	utils::zip::data_t buffer;

	// 	const size_t BUFSIZE = 128 * 1024;
	// 	uint8_t temp_buffer[BUFSIZE];

	// 	z_stream strm;
	// 	strm.zalloc = 0;
	// 	strm.zfree = 0;
	// 	strm.next_in = reinterpret_cast<uint8_t *>(in_data);
	// 	strm.avail_in = in_data_size;
	// 	strm.next_out = temp_buffer;
	// 	strm.avail_out = BUFSIZE;

	// 	deflateInit(&strm, Z_BEST_COMPRESSION);

	// 	while (strm.avail_in != 0)
	// 	{
	// 		int res = deflate(&strm, Z_NO_FLUSH);
	// 		assert(res == Z_OK);
	// 		if (strm.avail_out == 0)
	// 		{
	// 			buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
	// 			strm.next_out = temp_buffer;
	// 			strm.avail_out = BUFSIZE;
	// 		}
	// 	}

	// 	int deflate_res = Z_OK;
	// 	while (deflate_res == Z_OK)
	// 	{
	// 		if (strm.avail_out == 0)
	// 		{
	// 			buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
	// 			strm.next_out = temp_buffer;
	// 			strm.avail_out = BUFSIZE;
	// 		}
	// 		deflate_res = deflate(&strm, Z_FINISH);
	// 	}

	// 	assert(deflate_res == Z_STREAM_END);
	// 	buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
	// 	deflateEnd(&strm);

	// 	out_data.swap(buffer);
	// }
}

namespace utils
{
	namespace zip
	{
		using data_t = std::vector<unsigned char>;

		template <typename DataT>
		int compress_data(const DataT& data_in, data_t& data_out)
		{
			// Check if the input data is not empty
			if (data_in.empty())
			{
				LOG_ERROR("Input data is empty.");
				return Z_DATA_ERROR;
			}

			z_stream stream;
			stream.zalloc = Z_NULL;
			stream.zfree = Z_NULL;
			stream.opaque = Z_NULL;

			// Use default compression level (Z_DEFAULT_COMPRESSION)
			int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
			if (ret != Z_OK)
			{
				LOG_ERROR("Failed to initialize zlib stream for compression.");
				return ret;
			}

			stream.avail_in = static_cast<uInt>(data_in.size());
			stream.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(data_in.data()));

			std::vector<unsigned char> buffer(1024); // Adjust the buffer size as needed
			data_out.clear();

			do
			{
				stream.avail_out = static_cast<uInt>(buffer.size());
				stream.next_out = buffer.data();

				// Compress the data
				ret = deflate(&stream, Z_FINISH);
				switch (ret)
				{
				case Z_STREAM_ERROR:
				case Z_MEM_ERROR:
					LOG_ERROR("Failed to compress data.");
					deflateEnd(&stream);
					return ret;
				}

				// Append the compressed data to the output
				data_out.insert(data_out.end(), buffer.begin(), buffer.begin() + (buffer.size() - stream.avail_out));
			} while (stream.avail_out == 0);

			// Clean up and finalize the compression
			deflateEnd(&stream);

			LOG_DEBUG("Data compressed successfully.");
			return Z_OK;
		}

		template <typename DataT>
		int decompress_data(const data_t& data_in, DataT& data_out)
		{
			// Check if the input data is not empty
			if (data_in.empty())
			{
				LOG_ERROR("Input data is empty.");
				return Z_DATA_ERROR;
			}

			z_stream stream;
			stream.zalloc = Z_NULL;
			stream.zfree = Z_NULL;
			stream.opaque = Z_NULL;
			stream.avail_in = static_cast<uInt>(data_in.size());
			stream.next_in = const_cast<Bytef *>(data_in.data());

			// Initialize the zlib stream for decompression
			int ret = inflateInit(&stream);
			if (ret != Z_OK)
			{
				LOG_ERROR("Failed to initialize zlib stream for decompression.");
				return ret;
			}

			std::vector<unsigned char> buffer(1024); // Adjust the buffer size as needed
			data_out.clear();

			do
			{
				stream.avail_out = static_cast<uInt>(buffer.size());
				stream.next_out = buffer.data();

				// Decompress the data
				ret = inflate(&stream, Z_NO_FLUSH);
				switch (ret)
				{
				case Z_NEED_DICT:
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					LOG_ERROR("Failed to decompress data.");
					inflateEnd(&stream);
					return ret;
				}

				// Append the decompressed data to the output
				data_out.insert(data_out.end(), buffer.begin(), buffer.begin() + (buffer.size() - stream.avail_out));
			} while (stream.avail_out == 0);

			// Clean up and finalize the decompression
			inflateEnd(&stream);

			LOG_DEBUG("Data decompressed successfully.");
			return Z_OK;
		}

		int decompress_binary(const data_t& data, data_t& out_data)
		{
			return decompress_data(data, out_data);
		}

		int decompress_text(const data_t& data, std::string& out_data)
		{
			return decompress_data(data, out_data);
		}

		int compress_text(const std::string& data, data_t& out)
		{
			return compress_data(data, out);
		}

		int compress_binary(const data_t& data, data_t& out)
		{
			return compress_data(data, out);
		}
	}
}
