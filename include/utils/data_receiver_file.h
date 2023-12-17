// data_receiver_file.h

#pragma once

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
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
		public:
			using base = receiver_base<Data_element_t>;
			using data_t = std::vector<Data_element_t>;
			enum receiver_file_error : int
			{
				write_error = receiver_error::count,
				count,
			};
			receiver_file(std::size_t size, const fs::path& file_path = {}) : m_fpath(file_path), m_target_size(size) {
				reset(size);
			}
			// Overrides
			std::size_t receive_impl(const Data_element_t* data, const std::size_t& size) override {
				auto before = this->size();
				auto goal = std::min(m_target_size - before, size);
				try
				{
					m_file.write(data, goal);
				}
				catch (const std::exception& e)
				{
					m_file.close();
					set_error(receiver_file_error::write_error);
				}
				catch (...)
				{
					m_file.close();
					set_error(receiver_file_error::write_error);
				}
				auto after = this->size();
				m_current_size = after;
				if (after == m_target_size)
					m_file.close();
				return after - before;
			}
			const Data_element_t* data() const override {
				m_buffer = file::contents<data_t>(m_fpath);
				return m_buffer.data();
			}
			// Currently stored data size
			size_t size() const override {
				if (m_file.is_open())
					return std::abs(m_file.tellp());
				else
					return m_current_size;
			}
			std::size_t target_size() const override {
				return m_target_size;
			}
			bool full() const override {
				return size() == m_target_size;
			}
			void reset(const std::size_t& size = 0) override {
				base::reset(size);
				m_current_size = 0;
				m_target_size = 0;
				m_file.open(m_fpath, std::ios::binary);
				grow(size);
			}
			void grow(const std::size_t& size) override {
				base::grow(size);
				m_target_size += size;
				bool is_open = m_file.is_open();
				if (is_open)
					m_file.close();
				fs::resize_file(m_fpath, m_target_size);
				if (is_open)
					m_file.open(m_fpath, std::ios::binary);
				// TODO: clear the file?
			}

		private:
			std::size_t m_target_size = 0;
			std::size_t m_current_size = 0;
			fs::path m_fpath;
			mutable std::ofstream m_file;
			mutable data_t m_buffer;
		};
	}
}