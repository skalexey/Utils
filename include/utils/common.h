#pragma once

#include <functional>

namespace utils
{
	using void_cb = std::function<void()>;
	using void_int_arg_cb = std::function<void(int)>;
}