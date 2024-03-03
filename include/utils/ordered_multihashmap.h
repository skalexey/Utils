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
			_map().emplace(__key, (int)_list().size());
			_list().push_back(__value);
			_keys().push_back(__key);
			return iterator(*this, _list().size() - 1);
		}
		std::pair<iterator, bool> rename(const TK& __old_key, const TK& __new_key) override {
			auto range = _map().equal_range(__old_key);
			if (range.first == range.second)
				return {end(), false};
			auto new_key_it = _map().find(__new_key);
			if (new_key_it != _map().end())
				return {{*this, new_key_it->second}, false};
			for (auto it = range.first; it != range.second; ++it)
			{
				auto i = it->second;
				_map().emplace(__new_key, i);
				_keys()[i] = __new_key;
			}
			auto index = range.first->second;
			// Remove late to avoid invalidating the iterators
			_map().erase(range.first, range.second);
			return {iterator(*this, index), true};
		}
		std::pair<iterator, iterator> equal_range(const TK& __key) {
			auto range = _map().equal_range(__key);
			if (range.first == range.second)
				return {end(), end()};
			auto it1 = iterator(*this, range.first);
			auto it2 = range.second == _map().end() ? end() : iterator(*this, range.second);
			return {it1, it2};
		}
	};
}
