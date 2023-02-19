#pragma once

#include <functional>

namespace utils
{
	struct terminator
	{
		terminator(std::function<void()> func) : m_func(func) { }
		~terminator() {
			if (m_func) {
				m_func();
			}
		}

		private:
			std::function<void()> m_func;
	};
}
