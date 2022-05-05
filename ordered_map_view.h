#pragma once

template <typename TL, typename TM, typename TK, typename TV>
class ordered_map_iterator
{
public:
	using value_t = std::pair<TK&, TV&>;
	using const_value_t = std::pair<const TK&, const TV&>;
	using list_it_t = typename TL::iterator;
	ordered_map_iterator(const TL& list, const TM& map, int index)
		: m_map(map)
		, m_list(list)
		, m_index(index)
	{}
	value_t operator *() {
		auto v = const_cast<const ordered_map_iterator*>(this)->operator*();
		return {const_cast<TK&>(v.first), const_cast<TV&>(v.second)};
	}
	const_value_t operator *() const {
		auto& n = m_list[m_index];
		auto it = m_map.find(n);
		assert(it != m_map.end() && "Storage management error. Should never be");
		auto& v = it->second;
		return const_value_t(n, v);
	}
	ordered_map_iterator& operator ++() {
		if (m_index != m_list.size() - 1)
			m_index++;
		return *this;
	}
	ordered_map_iterator operator ++(int) {
		auto r = *this;
		if (m_index != m_list.size() - 1)
			m_index++;
		return r;
	}
	inline bool operator != (const ordered_map_iterator& y) {
		return (m_index != y.m_index);
	}
private:
	const TM& m_map;
	const TL& m_list;
	int m_index = -1;
};

// ordered_map_view
// View-like type for being able to use std::for_each with custom iteration logic
template <typename TL, typename TM, typename TK, typename TV>
class ordered_map_view
{
public:
	using iterator = ordered_map_iterator<TL, TM, TK, TV>;
	
	ordered_map_view(TL& l, TM& m)
		: m_list(l)
		, m_map(m)
	{}
	ordered_map_view(const TL& l, const TM& m)
		: m_list(l)
		, m_map(m)
	{}
	
	iterator begin() const {
		return iterator(m_list, m_map, empty() ? -1 : 0);
	}
	iterator end() const {
		return iterator(m_list, m_map, m_list.size() - 1);
	}
	iterator begin() {
		return iterator(m_list, m_map, empty() ? -1 : 0);
	}
	iterator end() {
		return iterator(m_list, m_map, m_list.size() - 1);
	}
	
	inline size_t size() const {
		return m_list.size();
	}
	bool empty() const {
		return size() == 0;
	}
	
private:
	const TL& m_list;
	const TM& m_map;
};
