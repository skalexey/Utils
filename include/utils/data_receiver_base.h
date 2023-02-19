// data_receiver_base.h

#pragma once

#include <cstddef>

namespace utils
{
	namespace data
	{
		// --- receiver_base ---
		template <typename Data_element_t>
		class receiver_base
		{
		public:
			virtual bool receive(const Data_element_t* data, const std::size_t& size) = 0;
			virtual const Data_element_t* data() const = 0;
			virtual size_t size() const = 0;
			virtual bool full() const = 0;
		};
	}
}

