#pragma once

#include <unordered_map>
#include <utils/ordered_map.h>

namespace utils
{
	template <typename TK, typename TV>
	class ordered_multihashmap : public ordered_hashmap<TK, TV, std::unordered_multimap<TK, int>>
	{
	public:
		using base = ordered_hashmap<TK, TV, std::unordered_multimap<TK, int>>;
		using iterator = typename base::iterator;
		
	private:
		using base::add;

	public:
		iterator add(const TK& __key, const TV& __value) {
			base::_map().emplace(__key, (int)base::_list().size());
			base::_list().push_back(__value);
			base::_keys().push_back(__key);
			return iterator(*this, base::_list().size() - 1);
		}
		std::pair<iterator, bool> rename(const TK& __old_key, const TK& __new_key) override {
			auto range = base::_map().equal_range(__old_key);
			if (range.first == range.second)
				return { base::end(), false};
			auto new_key_it = base::_map().find(__new_key);
			if (new_key_it != base::_map().end())
				return {{*this, new_key_it->second}, false};
			for (auto it = range.first; it != range.second; ++it)
			{
				auto i = it->second;
				base::_map().emplace(__new_key, i);
				base::_keys()[i] = __new_key;
			}
			auto index = range.first->second;
			// Remove late to avoid invalidating the iterators
			base::_map().erase(range.first, range.second);
			return {iterator(*this, index), true};
		}
		std::pair<iterator, iterator> equal_range(const TK& __key) {
			auto range = base::_map().equal_range(__key);
			if (range.first == range.second)
				return { base::end(), base::end()};
			auto it1 = iterator(*this, range.first);
			auto it2 = range.second == base::_map().end() ? base::end() : iterator(*this, range.second);
			return {it1, it2};
		}
	};
}
