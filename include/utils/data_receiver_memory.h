// data_receiver_memory.h

#pragma once

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
			void receive_impl(const Data_element_t* data, const std::size_t& size) override {
				m_data.insert(m_data.end(), data, data + size);
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
				m_data.clear();
				m_data.reserve(size);
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