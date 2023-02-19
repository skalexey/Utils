// data_receiver_memory.h

#pragma once

#include <exception>
#include <vector>
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

		public:
			receiver_memory(const std::size_t& size) { m_data.reserve(size); }
			bool receive(const Data_element_t* data, const std::size_t& size) override {
				assert(size + m_data.size() <= m_data.capacity());
				try
				{
					m_data.insert(m_data.end(), data, data + size);
				}
				catch (const std::exception& e)
				{
					return false;
				}
				catch (...)
				{
					return false;
				}
				return true;
			}
			const Data_element_t* data() const override { return m_data.data(); }
			size_t size() const override { return m_data.size(); }
			const data_t& data_container() const { return m_data; }
			bool full() const override {
				return size() == m_data.capacity();
			}
		private:
			data_t m_data;
		};
	}
}