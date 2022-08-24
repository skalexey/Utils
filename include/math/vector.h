#pragma once
#include <array>
#include <cmath>

namespace math
{
	#define FOREACH_COMPONENT for (int i = 0; i < m_data.size(); i++)
	#define ACCUMULATE(ex) auto sum = T(); FOREACH_COMPONENT sum += (ex);
	#define rt_this static_cast<RT&>(*this)	
	#define rt_const_this static_cast<const RT&>(*this)

	template <typename T, const int S, typename RT>
	class vector
	{
	protected:
		using data_t = std::array<T, S>;
		using vector_t = vector<T, S, RT>;

	public:
		// Constructors and assignment operators
		vector() : m_data({}) {}
		vector(const data_t& data) : m_data(data) {}
		RT& operator = (const RT& r) { m_data = r.m_data; return rt_this; }
		// Value returning operations
		RT operator + (const RT& r) const { return RT(rt_const_this) += r; }
		RT operator - (const RT& r) const { return RT(rt_const_this) -= r; };
		RT operator - () const { return RT(rt_const_this) *= T(-1); }
		RT operator * (const RT& r) const { return RT(rt_const_this) *= r; }
		RT operator / (const RT& r) const { return RT(rt_const_this) /= r; }
		RT normalized() const { return RT(rt_const_this).normalize(); }
		RT proj(const RT& vr) const {
			auto sqlen_vr = vr.sqlength();
			if (sqlen_vr == 0.f)
				return vr;
			return vr * RT(rt_const_this).dot(vr) / sqlen_vr;
		}
		template <typename R>
		RT operator * (const R& r) const { return RT(rt_const_this) *= T(r); }
		template <typename R>
		RT operator / (const R& r) const { return RT(rt_const_this) /= T(r); }

		// Reference returning operations
		RT& operator += (const RT& r) {
			FOREACH_COMPONENT
				data(i) += r.data(i);
			return rt_this;
		}
		RT& operator -= (const RT& r) {
			FOREACH_COMPONENT
				data(i) -= r.data(i);
			return rt_this;
		}
		template <class R>
		RT& operator *= (const R& s) {
			FOREACH_COMPONENT
				data(i) *= s;
			return rt_this;
		}
		template <class R>
		RT& operator /= (const R& s) {
			FOREACH_COMPONENT
				data(i) /= s;
			return rt_this;
		}
		RT& operator *= (const RT& r) const {
			FOREACH_COMPONENT
				data(i) *= r.data(i);
			return rt_this;
		}
		RT& operator /= (const RT& r) const {
			FOREACH_COMPONENT
				data(i) /= r.data(i);
			return rt_this;
		}
		RT& normalize() { return operator/=(length()); }
		RT& normalizeTo(T len) { return normalize(); *this *= len; }

		// Bool operators
		bool operator == (const RT& r) const {
			FOREACH_COMPONENT
				if (data(i) != r.data(i))
					return false;
			return true;
		}
		bool operator > (const vector_t& r) const { return sqlength() > r.sqlength(); }
		bool operator < (const vector_t& r) const { return sqlength() < r.sqlength(); }
		bool operator != (const vector_t& r) const { return !operator==(r); }

		// Value returning operations
		T length() const { return (T)std::sqrt(sqlength()); }
		T sqlength() const { return dot(*this); }
		T manhattanLength() const {	ACCUMULATE(std::abs(data(i))); return sum; }
		T dot(const vector_t& vr) const { ACCUMULATE(data(i) * vr.data(i)); return sum;	}

	protected:
		const T& data(std::size_t index) const { return m_data[index]; }
		T& data(std::size_t index) { return m_data[index]; }

	protected:
		data_t m_data;
	};
}

