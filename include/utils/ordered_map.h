#pragma once

#include <utility>
#include <vector>
#include <unordered_map>

namespace utils
{
	template <typename TK, typename TV>
	class ordered_map_interface;

	template <typename TK, typename TV>
	class ordered_map_terator
	{
	public:
		using TM = ordered_map_interface<TK, TV>;
		using value_t = std::pair<TK&, TV&>;
		using const_value_t = std::pair<const TK&, const TV&>;
		ordered_map_terator(const TM& __map, int __index)
			: m_map(__map)
			, m_index(__index)
		{}
		inline value_t operator *() {
			auto v = const_cast<const ordered_map_terator*>(this)->operator*();
			return {const_cast<TK&>(v.first), const_cast<TV&>(v.second)};
		}
		inline const_value_t operator *() const {
			auto it = m_map.begin() + m_index;
			if (!it._valid())
				throw "ordered_map_terator: Invalid iterator dereference";
			auto& n = m_map.get_key_at(it.m_index);
			auto& v = m_map.get_value(n);
			return const_value_t(n, v);
		}
		inline ordered_map_terator& operator ++() {
			if (m_index < m_map.size() - 1)
				m_index++;
			else if (m_index != -1)
				m_index = -1;
			return *this;
		}
		inline ordered_map_terator operator ++(int) {
			auto r = *this;
			operator ++();
			return r;
		}
		inline ordered_map_terator operator +(int __diff) const {
			if (!_valid())
				return m_map.end();
			auto it = ordered_map_terator(m_map, m_index + __diff);
			if (it._valid())
				return it;
			return m_map.end();
		}
		inline ordered_map_terator operator -(int __diff) const {
			if (!_valid())
				return m_map.end();
			auto it = ordered_map_terator(m_map, m_index - __diff);
			if (it._valid())
				return it;
			return m_map.end();
		}
		inline bool operator != (const ordered_map_terator& y) {
			return (m_index != y.m_index);
		}
		inline bool operator == (const ordered_map_terator& y) {
			return (m_index == y.m_index);
		}
		
	protected:
		inline bool _valid() const {
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
		using iterator = ordered_map_terator<TK, TV>;
		
		inline iterator begin() const {
			if (empty())
				return end();
			return iterator(*this, 0);
		}
		inline iterator end() const {
			return iterator(*this, -1);
		}
		inline iterator begin() {
			return std::as_const(*this).begin();
		}
		inline iterator end() {
			return std::as_const(*this).end();
		}
		inline iterator find(const TK& __key) const {
			auto it = _get_map().find(__key);
			if (it == _get_map().end())
				return end();
			return iterator(*this, it->second);
		}
		inline iterator find(const TK& key) {
			return const_cast<const ordered_map_interface*>(this)->find(key);
		}
		inline const TV& get_value(const TK& __key) const {
			auto it = _get_map().find(__key);
			return _get_list()[it->second];
		}
		inline TV& value(const TK& __key) {
			return const_cast<TV&>(const_cast<ordered_map_interface*>(this)->get_value(__key));
		}
		inline const TK& get_key_at(int __index) const {
			return _get_keys()[__index];
		}
		inline int get_index(const TK& k) const {
			auto it = _get_map().find(k);
			if (it == _get_map().end())
				return -1;
			return it->second;
		}
		inline size_t size() const {
			return _get_list().size();
		}
		inline bool empty() const {
			return size() == 0;
		}
		inline TV& add(const TK& __key, const TV& __value) {
			auto res = _map().emplace(__key, _list().size());
			_list().push_back(__value);
			_keys().push_back(__key);
			return _list().back();
		}
		inline TV& set(const TK& __key, const TV& __value) {
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
		inline bool erase(const TK& k) {
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
		inline bool erase_at(int __index) {
			if (__index < 0 || __index >= size())
				return false;
			erase(_get_keys(__index));
		}

	protected:
		// Pure virtuals
		virtual inline const TL& _get_list() const = 0;
		virtual inline const TM& _get_map() const = 0;
		virtual inline const TKI& _get_keys() const = 0;
		
		// Non-const versions
		inline TL& _list() {
			return const_cast<TL&>(_get_list());
		}
		inline TM& _map() {
			return const_cast<TM&>(_get_map());
		}
		inline TKI& _keys() {
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
		inline const TL& _get_list() const override
		{
			return m_list;
		}

		inline const TM& _get_map() const override
		{
			return m_map;
		}
		
		inline const TKI& _get_keys() const override
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
		inline ordered_map_view(const TOM& __map)
			: m_map(__map)
		{}

	protected:
		inline const TL& _get_list() const override
		{
			return m_map._get_list();
		}

		inline const TM& _get_map() const override
		{
			return m_map._get_map();
		}
		
		inline const TKI& _get_keys() const override
		{
			return m_map._get_keys();
		}

	private:
		const TOM& m_map;
	};
}
