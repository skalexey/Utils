// data_receiver_memory.h

#pragma once

#include <exception>
#include <vector>
#include <cassert>
#include <utils/data_receiver_base.h>

namespace utils
{
	namespace data
	{
		// --- receiver_memory ---
		template <typename Data_element_t>
		class receiver_memory : public receiver_base<Data_element_t>
		{
		public:
			using base = receiver_base<Data_element_t>;
			using data_t = std::vector<Data_element_t>;
			enum receiver_memory_error : int
			{
				count,
			};

		public:
			receiver_memory(const std::size_t& size = 0) {
				reset(size);
			}
			const data_t& data_container() const {
				return m_data;
			}
			// Overrides
			std::size_t receive_impl(const Data_element_t* data, const std::size_t& size) override {
				auto before = m_data.size();
				try
				{
					m_data.insert(m_data.end(), data, data + size);
					return size;
				}
				catch (const std::exception& ex)
				{
					return m_data.size() - before;
				}
			}
			const Data_element_t* data() const override {
				return m_data.data();
			}
			size_t size() const override {
				return m_data.size();
			}
			std::size_t target_size() const override {
				return m_data.capacity();
			}
			bool full() const override {
				return size() == m_data.capacity();
			}
			void reset(const std::size_t& size = 0) override {
				base::reset(size);
				m_data = decltype(m_data)();
				m_data.reserve(size);
				auto cap = m_data.capacity();
				assert(cap == size);
			}
			void grow(const std::size_t& size) {
				base::grow(size);
				m_data.reserve(m_data.capacity() + size);
			}

		private:
			data_t m_data;
		};
	}
}