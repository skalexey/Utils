#pragma once

#include <functional>

namespace utils
{
	using void_cb = std::function<void()>;
	using int_cb = std::function<int()>;
	using void_int_cb = std::function<void(int)>;
	using void_bool_cb = std::function<void(bool)>;
	using void_string_bool_cb = std::function<void(const std::string&, bool)>;
}