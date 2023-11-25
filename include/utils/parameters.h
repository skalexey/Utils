#pragma once

#include <variant>
#include <utility>
#include <tuple>

namespace utils
{
	template<size_t N, typename... Args>
	auto get_n_pack_arg(Args&&... args)
	{
		return std::get<N>(std::tuple{std::forward<Args>(args)...});
	}
}