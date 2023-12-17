// data_receiver_base.h

#pragma once

#include <cstddef>
#include <exception>
#include <cassert>

namespace utils
{
	namespace data
	{
		// --- receiver_base ---
		template <typename Data_element_t>
		class receiver_base
		{
		public:
			enum receiver_error : int
			{
				std_exception = 1,
				unknown_exception,
				partial_receive,
				count,
			};
			using data_element_t = Data_element_t;

			std::size_t receive(const Data_element_t* data, const std::size_t& size) {
				auto before = this->size();
				auto goal = m_is_unlimited ? size : std::min(target_size() - before, size);
				std::size_t received = 0;
				try
				{
					received = receive_impl(data, goal);
				}
				catch (const std::exception& e)
				{
					m_error = receiver_error::std_exception;
					m_error_message = e.what();
				}
				catch (...)
				{
					m_error = receiver_error::unknown_exception;
				}
				auto after = this->size();
				//auto received = after - before;
				assert(received == goal); // Written less than expected
				if (!m_is_unlimited)
				{
					if (received != goal)
						m_error = receiver_error::partial_receive;
				}
				return received;
			}
			virtual const Data_element_t* data() const = 0;
			virtual size_t size() const = 0;
			virtual std::size_t target_size() const = 0;
			virtual bool full() const = 0;
			virtual bool empty() const {
				return size() == 0;
			}
			virtual void reset(const std::size_t& size = 0) {
				m_error = 0;
				m_is_unlimited = size == 0;
			}
			virtual void grow(const std::size_t& size) {
				if (size > 0)
					m_is_unlimited = false;
			}
			bool is_unlimited() const {
				return m_is_unlimited;
			}
			int error_code() const {
				return m_error;
			}
			const std::string& error_message() const {
				return m_error_message;
			}
			template <typename Container_t>
			Container_t data_container() const {
				return Container_t(data(), data() + size());
			}

		protected:
			virtual std::size_t receive_impl(const Data_element_t* data, const std::size_t& size) = 0;
			void set_error(const int& error) {
				m_error = error;
			}
			
		private:
			int m_error = 0;
			std::string m_error_message;
			bool m_is_unlimited = false;
		};
	}
}

