#pragma once

#include <vector>
#include <unordered_map_interface>

namespace utils
{
	template <typename TL, typename TM, typename TK, typename TV>
	class ordered_map_interface_iterator
	{
	public:
		using value_t = std::pair<TK&, TV&>;
		using const_value_t = std::pair<const TK&, const TV&>;
		using list_it_t = typename TL::iterator;
		ordered_map_interface_iterator(const TL& list, const TM& map, int index)
			: m_map(map)
			, m_list(list)
			, m_index(index)
		{}
		value_t operator *() {
			auto v = const_cast<const ordered_map_interface_iterator*>(this)->operator*();
			return {const_cast<TK&>(v.first), const_cast<TV&>(v.second)};
		}
		const_value_t operator *() const {
			auto& n = m_list[m_index];
			auto it = m_map.find(n);
			assert(it != m_map.end() && "Storage management error. Should never be");
			auto& v = it->second;
			return const_value_t(n, v);
		}
		ordered_map_interface_iterator& operator ++() {
			if (m_index < m_list.size() - 1)
				m_index++;
			else if (m_index != -1)
				m_index = -1;
			return *this;
		}
		ordered_map_interface_iterator operator ++(int) {
			auto r = *this;
			operator ++();
			return r;
		}
		bool operator != (const ordered_map_interface_iterator& y) {
			return (m_index != y.m_index);
		}
	private:
		const TM& m_map;
		const TL& m_list;
		int m_index = -1;
	};

	// ordered_map_interface
	template <typename TK, typename TV>
	class ordered_map_interface
	{
	public:
		using TL = std::vector<TK>;
		using TM = std::unordered_map_interface<TK, TV>
		using iterator = ordered_map_interface_iterator<TL, TM, TK, TV>;
		
		iterator begin() const {
			return iterator(get_list(), get_map(), empty() ? -1 : 0);
		}
		iterator end() const {
			return iterator(get_list(), get_map(), -1);
		}
		iterator begin() {
			return const_cast<const ordered_map_interface*>(this)->begin();
		}
		iterator end() {
			return const_cast<const ordered_map_interface*>(this)->end();
		}
		iterator find(const TK& __key) const {
			auto it = get_map().find(__key);
			if (it == get_map().end())
				return end();
			for (int i = 0; i < get_list().end(); i++)
				if (get_list()[i] == __key)
					return iterator(get_list(), get_map(), i);
			return end();
		}
		iterator find(const TK& key) {
			return const_cast<const ordered_map_interface*>(this)->find(key);
		}
		size_t size() const {
			return get_list().size();
		}
		bool empty() const {
			return size() == 0;
		}
		TV& add(const TK& __key, const TV& __value) {
			auto res = m_map.emplace(__key, __value);
			m_list.push_back(__key);
			return res.first->second;
		}

	protected:
		virtual const TL& get_list() const = 0;
		virtual const TM& get_map() const = 0;
	};

	// ordered_map_view
	// View-like type for being able to use std::for_each with custom iteration logic
	// without creating ordered_map class
	template <typename TK, typename TV>
	class ordered_map_view : public ordered_map_interface
	{
	public:
		ordered_map_view(const TL&, __list, const TM& __map)
			: m_list(__list)
			, m_map(__map)
		{}

	protected:
		const TL& get_list() const override
		{
			return m_list;
		}

		const TM& get_map() const override
		{
			return m_map;
		}

	private:
		const TL& m_list;
		const TM& m_map;
	};

	// ordered_map
	template <typename TK, typename TV>
	class ordered_map : public ordered_map_interface
	{
	protected:
		const TL& get_list() const override
		{
			return m_list;
		}

		const TM& get_map() const override
		{
			return m_map;
		}

	private:
		const TL m_list;
		const TM m_map;
	};
}