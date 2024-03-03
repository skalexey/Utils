#pragma once

#include <utility>
#include <vector>
#include <unordered_map>

namespace utils
{
	template <typename TK, typename TV, typename TC>
	class ordered_map_interface;

	template <typename TK, typename TV, typename TC>
	class ordered_map_iterator
	{
	public:
		using iterator_t = ordered_map_iterator<TK, TV, TC>;
		using TM = ordered_map_interface<TK, TV, TC>;
		using value_t = std::pair<TK&, TV&>;
		using const_value_t = std::pair<const TK&, const TV&>;

		ordered_map_iterator(const TM& __map, int __index)
			: m_map_ptr(&__map)
			, m_index(__index)
		{}
		value_t operator*() {
			auto v = const_cast<const ordered_map_iterator*>(this)->operator*();
			return {const_cast<TK&>(v.first), const_cast<TV&>(v.second)};
		}
		const_value_t operator*() const {
			auto it = map().begin() + m_index;
			if (!it._valid())
				throw "ordered_map_iterator: Invalid iterator dereference";
			auto& n = map().get_key_at(it.m_index);
			auto& v = map().get_value(n);
			return const_value_t(n, v);
		}
		ordered_map_iterator& operator++() {
			if (m_index < map().size() - 1)
				m_index++;
			else if (m_index != -1)
				m_index = -1;
			return *this;
		}
		ordered_map_iterator operator++(int) {
			auto r = *this;
			operator++();
			return r;
		}
		ordered_map_iterator operator+(int __diff) const {
			if (!_valid())
				return map().end();
			auto it = ordered_map_iterator(map(), m_index + __diff);
			if (it._valid())
				return it;
			return map().end();
		}
		ordered_map_iterator operator-(int __diff) const {
			if (!_valid())
				return map().end();
			auto it = ordered_map_iterator(map(), m_index - __diff);
			if (it._valid())
				return it;
			return map().end();
		}
		bool operator!=(const ordered_map_iterator& y) const {
			return (m_index != y.m_index);
		}
		bool operator==(const ordered_map_iterator& y) const {
			return (m_index == y.m_index);
		}
		operator bool() const {
			return !operator==(map().end());
		}
		
	protected:
		bool _valid() const {
			return m_index >= 0 && m_index < map().size();
		}
		const TM& map() const {
			return *m_map_ptr;
		}
		
	protected:
		int m_index = -1;

	private:
		const TM* m_map_ptr = nullptr;
	};

	template <typename TK, typename TV, typename TC>
	class ordered_map_reverse_iterator : public ordered_map_iterator<TK, TV, TC>
	{
	public:
		using base = ordered_map_iterator<TK, TV, TC>;
		ordered_map_reverse_iterator(const base::TM& __map, int __index)
			: base(__map, __index)
		{}
		ordered_map_reverse_iterator& operator++() {
			if (m_index >= 0)
				m_index--;
			return *this;
		}
		ordered_map_reverse_iterator operator++(int) {
			auto r = *this;
			operator++();
			return r;
		}
		ordered_map_reverse_iterator operator+(int __diff) const {
			if (!_valid())
				return map().rend();
			auto it = ordered_map_reverse_iterator(map(), m_index - __diff);
			if (it._valid())
				return it;
			return map().rend();
		}
		ordered_map_reverse_iterator operator-(int __diff) const {
			if (!_valid())
				return map().rend();
			auto it = ordered_map_reverse_iterator(map(), m_index + __diff);
			if (it._valid())
				return it;
			return map().rend();
		}
		operator bool() const {
			return !operator==(map().rend());
		}
	};

	// ordered_map_interface
	template <typename TK, typename TV, typename TC>
	class ordered_map_interface
	{
	public:
		using TL = std::vector<TV>;
		using TM = TC;
		using TKI = std::vector<TK>;
		
	public:
		using iterator = ordered_map_iterator<TK, TV, TC>;
		using reverse_iterator = ordered_map_reverse_iterator<TK, TV, TC>;

		iterator begin() const {
			if (empty())
				return end();
			return iterator(*this, 0);
		}
		iterator end() const {
			return iterator(*this, -1);
		}
		iterator begin() {
			return std::as_const(*this).begin();
		}
		iterator end() {
			return std::as_const(*this).end();
		}
		reverse_iterator rbegin() const {
			if (empty())
				return rend();
			return reverse_iterator(*this, size() - 1);
		}
		reverse_iterator rend() const {
			return reverse_iterator(*this, -1);
		}
		const TL& get_list() const {
			return _get_list();
		}
		const TKI& get_keys() const {
			return _get_keys();
		}
		iterator find(const TK& __key) const {
			auto it = _get_map().find(__key);
			if (it == _get_map().end())
				return end();
			return iterator(*this, it->second);
		}
		iterator find(const TK& key) {
			return const_cast<const ordered_map_interface*>(this)->find(key);
		}
		const TV& get_value(const TK& __key) const {
			auto it = _get_map().find(__key);
			return _get_list()[it->second];
		}
		TV& value(const TK& __key) {
			return const_cast<TV&>(const_cast<ordered_map_interface*>(this)->get_value(__key));
		}
		TV& at(int __index) {
			auto& k = get_key_at(__index);
			return value(k);
		}
		const TV& get_at(int __index) const {
			auto& k = get_key_at(__index);
			return get_value(k);
		}
		const TK& get_key_at(int __index) const {
			return _get_keys()[__index];
		}
		int get_index(const TK& k) const {
			auto it = _get_map().find(k);
			if (it == _get_map().end())
				return -1;
			return it->second;
		}
		size_t size() const {
			return _get_list().size();
		}
		bool empty() const {
			return size() == 0;
		}
		bool has(const TK& k) const {
			return get_index(k) != -1;
		}
		void clear() {
			_list().clear();
			_map().clear();
			_keys().clear();
		}
		std::pair<iterator, bool> add(const TK& __key, const TV& __value) {
			auto res = _map().emplace(__key, (int)_list().size());
			if (!res.second)
				return {{*this, res.first->second}, false};
			_list().push_back(__value);
			_keys().push_back(__key);
			return {iterator(*this, _list().size() - 1), true};
		}
		iterator set(const TK& __key, const TV& __value) {
			auto index = get_index(__key);
			if (index < 0)
				return add(__key, __value);
			else
			{
				auto& r = _list()[index];
				r = __value;
				return iterator(*this, index);
			}
		}
		virtual std::pair<iterator, bool> rename(const TK& __old_key, const TK& __new_key) {
			auto it = _map().find(__old_key);
			if (it == _map().end())
				return {end(), false};
			auto new_key_it = _map().find(__new_key);
			if (new_key_it != _map().end())
				return {{*this, new_key_it->second}, false};
			auto i = it->second;
			_map().erase(it);
			_map().emplace(__new_key, i);
			_keys()[i] = __new_key;
			return {iterator(*this, i), true};
		}
		bool erase(const TK& k) {
			auto it = _map().find(k);
			if (it == _map().end())
				return false;
			auto i = it->second;
			_map().erase(it);
			_list().erase(_list().begin() + i);
			_keys().erase(_keys().begin() + i);
			for (auto& [k,v] : _map())
				if (v > i)
					v--;
			return true;
		}
		iterator erase(iterator __it) {
			if (!__it)
				return end();
			auto it = _map().find(__it->first);
			if (it == _map().end())
				return end();
			auto i = it->second;
			_map().erase(it);
			_list().erase(_list().begin() + i);
			_keys().erase(_keys().begin() + i);
			for (auto& [k,v] : _map())
				if (v > i)
					v--;
			return iterator(*this, i < size() ? i : -1);
		}
		bool erase_at(int __index) {
			if (__index < 0 || __index >= size())
				return false;
			erase(_get_keys(__index));
		}
		void reserve(size_t __size) {
			_list().reserve(__size);
			_keys().reserve(__size);
		}

	protected:
		// Pure virtuals
		virtual const TL& _get_list() const = 0;
		virtual const TM& _get_map() const = 0;
		virtual const TKI& _get_keys() const = 0;
		
		// Non-const versions
		TL& _list() {
			return const_cast<TL&>(_get_list());
		}
		TM& _map() {
			return const_cast<TM&>(_get_map());
		}
		TKI& _keys() {
			return const_cast<TKI&>(_get_keys());
		}
	};

	template <typename TK>
	using ordered_hashmap_t = std::unordered_map<TK, int>;

	template <typename TK, typename TV, typename TC = ordered_hashmap_t<TK>>
	class ordered_hashmap_view;

	// ordered_map
	template <typename TK, typename TV, typename TC = ordered_hashmap_t<TK>>
	class ordered_hashmap : public ordered_map_interface<TK, TV, TC>
	{
		using base = ordered_map_interface<TK, TV, TC>;
		
		friend class ordered_hashmap_view<TK, TV, TC>;
		
	public:
		using TL = typename base::TL;
		using TM = typename base::TM;
		using TKI = typename base::TKI;
		
	protected:
		const TL& _get_list() const override
		{
			return m_list;
		}

		const TM& _get_map() const override
		{
			return m_map;
		}
		
		const TKI& _get_keys() const override
		{
			return m_keys;
		}

	private:
		TL m_list;
		TM m_map;
		TKI m_keys;
	};

	// ordered_hashmap_view
	// View-like type for being able to use std::for_each with custom iteration logic
	// without creating ordered_map class
	template <typename TK, typename TV, typename TC>
	class ordered_hashmap_view : public ordered_map_interface<TK, TV, TC>
	{
	public:
		using TOM = ordered_hashmap<TK, TV, TC>;
		using TL = typename TOM::TL;
		using TM = typename TOM::TM;
		using TKI = typename TOM::TKI;
		
	public:
		ordered_hashmap_view(const TOM& __map)
			: m_map(__map)
		{}

	protected:
		const TL& _get_list() const override
		{
			return m_map._get_list();
		}

		const TM& _get_map() const override
		{
			return m_map._get_map();
		}
		
		const TKI& _get_keys() const override
		{
			return m_map._get_keys();
		}

	private:
		const TOM& m_map;
	};
}
