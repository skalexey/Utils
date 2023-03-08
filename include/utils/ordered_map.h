#pragma once

#include <utility>
#include <vector>
#include <unordered_map>

namespace utils
{
	template <typename TK, typename TV>
	class ordered_map_interface;

	template <typename TK, typename TV>
	class ordered_map_iterator
	{
	public:
		using TM = ordered_map_interface<TK, TV>;
		using value_t = std::pair<TK&, TV&>;
		using const_value_t = std::pair<const TK&, const TV&>;
		ordered_map_iterator(const TM& __map, int __index)
			: m_map(__map)
			, m_index(__index)
		{}
		value_t operator *() {
			auto v = const_cast<const ordered_map_iterator*>(this)->operator*();
			return {const_cast<TK&>(v.first), const_cast<TV&>(v.second)};
		}
		const_value_t operator *() const {
			auto it = m_map.begin() + m_index;
			if (!it._valid())
				throw "ordered_map_iterator: Invalid iterator dereference";
			auto& n = m_map.get_key_at(it.m_index);
			auto& v = m_map.get_value(n);
			return const_value_t(n, v);
		}
		ordered_map_iterator& operator ++() {
			if (m_index < m_map.size() - 1)
				m_index++;
			else if (m_index != -1)
				m_index = -1;
			return *this;
		}
		ordered_map_iterator operator ++(int) {
			auto r = *this;
			operator ++();
			return r;
		}
		ordered_map_iterator operator +(int __diff) const {
			if (!_valid())
				return m_map.end();
			auto it = ordered_map_iterator(m_map, m_index + __diff);
			if (it._valid())
				return it;
			return m_map.end();
		}
		ordered_map_iterator operator -(int __diff) const {
			if (!_valid())
				return m_map.end();
			auto it = ordered_map_iterator(m_map, m_index - __diff);
			if (it._valid())
				return it;
			return m_map.end();
		}
		bool operator != (const ordered_map_iterator& y) const {
			return (m_index != y.m_index);
		}
		bool operator == (const ordered_map_iterator& y) const {
			return (m_index == y.m_index);
		}
		operator bool() const {
			return !operator==(m_map.end());
		}
		
	protected:
		bool _valid() const {
			return m_index >= 0 && m_index < m_map.size();
		}
		
	private:
		const TM& m_map;
		int m_index = -1;
	};

	// ordered_map_interface
	template <typename TK, typename TV>
	class ordered_map_interface
	{
	public:
		using TL = std::vector<TV>;
		using TM = std::unordered_map<TK, int>;
		using TKI = std::vector<TK>;
		
	public:
		using iterator = ordered_map_iterator<TK, TV>;
		
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
		const TV& get_at(int __index) {
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
		void clear() {
			_list().clear();
			_map().clear();
			_keys().clear();
		}
		TV& add(const TK& __key, const TV& __value) {
			auto res = _map().emplace(__key, _list().size());
			_list().push_back(__value);
			_keys().push_back(__key);
			return _list().back();
		}
		TV& set(const TK& __key, const TV& __value) {
			auto index = get_index(__key);
			if (index < 0)
				return add(__key, __value);
			else
			{
				auto& r = _list()[index];
				r = __value;
				return r;
			}
		}
		bool rename(const TK& __old_key, const TK& __new_key) {
			auto it = _map().find(__old_key);
			if (it == _map().end())
				return false;
			auto i = it->second;
			_map().erase(it);
			_map().emplace(__new_key, i);
			_keys()[i] = __new_key;
			return true;
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
					i--;
			return true;
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

	template <typename TK, typename TV>
	class ordered_map_view;

	// ordered_map
	template <typename TK, typename TV>
	class ordered_map : public ordered_map_interface<TK, TV>
	{
		using base = ordered_map_interface<TK, TV>;
		
		friend class ordered_map_view<TK, TV>;
		
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

	// ordered_map_view
	// View-like type for being able to use std::for_each with custom iteration logic
	// without creating ordered_map class
	template <typename TK, typename TV>
	class ordered_map_view : public ordered_map_interface<TK, TV>
	{
	public:
		using TOM = ordered_map<TK, TV>;
		using TL = typename TOM::TL;
		using TM = typename TOM::TM;
		using TKI = typename TOM::TKI;
		
	public:
		ordered_map_view(const TOM& __map)
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
