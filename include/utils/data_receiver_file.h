// data_receiver_file.h

#pragma once

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <utils/filesystem.h>
#include <utils/data_receiver_base.h>
#include <utils/file_utils.h>

namespace utils
{
	namespace data
	{
		// --- receiver_file ---
		template <typename Data_element_t>
		class receiver_file : public receiver_base<Data_element_t>
		{
			using base = receiver_base<Data_element_t>;
			using data_t = std::vector<Data_element_t>;
			std::size_t m_target_size = 0;
			std::string m_fpath;
			mutable std::ofstream m_file;
			mutable data_t m_buffer;

		public:
			receiver_file(std::size_t size, const std::string& file_path = "") : m_fpath(file_path), m_target_size(size) {
				fs::resize_file(m_fpath, size);
				m_file.open(m_fpath, std::ios::binary);
			}
			bool receive(const Data_element_t* data, const std::size_t& size) override {
				auto before = this->size();
				assert(size + before <= m_target_size);
				m_file.write(data, size);
				auto after = this->size();
				auto result = after - before == size;
				assert(result);
				return result;
			}
			const Data_element_t* data() const override {
				m_buffer = file::contents<data_t>(m_fpath);
				return m_buffer.data();
			}
			// Currently stored data size
			size_t size() const override {
				return std::abs(m_file.tellp());
			}
		};
	}
}