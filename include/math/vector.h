#pragma once
#include <array>
#include <cmath>

namespace math
{
	#define FOREACH_COMPONENT for (int i = 0; i < m_data.size(); i++)
	#define ACCUMULATE(ex) auto sum = T(); FOREACH_COMPONENT sum += (ex);

	template <typename T, const int S>
	class vector
	{
	protected:
		using data_t = std::array<T, S>;
		using vector_t = vector<T, S>;

	public:
		// Constructors and assignment operators
		vector() : m_data({}) {}
		vector(const data_t& data) : m_data(data) {}

		// Value returning operations
		vector_t operator + (const vector_t& r) const { return vector_t(*this) += r; }
		vector_t operator - (const vector_t& r) const { return vector_t(*this) -= r; };
		vector_t operator - () const { return vector_t(*this) *= T(-1); }
		vector_t operator * (const vector_t& r) const { return vector_t(*this) *= r; }
		vector_t operator / (const vector_t& r) const { return vector_t(*this) /= r; }
		vector_t normalized() const { return vector_t(*this).normalize(); }
		template <typename R>
		vector_t operator * (const R& r) const { return vector_t(*this) *= T(r); }
		template <typename R>
		vector_t operator / (const R& r) const { return vector_t(*this) /= T(r); }

		// Reference returning operations
		vector_t& operator += (const vector_t& r) {
			FOREACH_COMPONENT
				data(i) += r.data(i);
			return *this;
		}
		vector_t& operator -= (const vector_t& r) {
			FOREACH_COMPONENT
				data(i) -= r.data(i);
			return *this;
		}
		template <class R>
		vector_t& operator *= (const R& s) {
			FOREACH_COMPONENT
				data(i) *= s;
			return (*this);
		}
		template <class R>
		vector_t& operator /= (const R& s) {
			FOREACH_COMPONENT
				data(i) /= s;
			return (*this);
		}
		vector_t& operator *= (const vector_t& r) const {
			FOREACH_COMPONENT
				data(i) *= r.data(i);
			return *this;
		}
		vector_t& operator /= (const vector_t& r) const {
			FOREACH_COMPONENT
				data(i) /= r.data(i);
			return *this;
		}
		vector_t& normalize() { return operator/=(length()); }
		vector_t& normalizeTo(T len) { return normalize(); *this *= len; }

		// Bool operators
		bool operator == (const vector_t& r) const {
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

	// Base macros
	#define OVERLOAD_OP(T, Base, op) T& op(const T& r) { Base::op(r); return *this; }
	#define OVERLOAD_OP_CUSTOM(T, Base, op) \
	template <typename C> \
	T& op(const C& r) { Base::op(r); return *this; }
	
	// Derived classes macros
	#define VEC_OVERLOAD_REF(op) OVERLOAD_OP(type, base, op)
	#define VEC_OVERLOAD_REF_CUSTOM(op) OVERLOAD_OP_CUSTOM(type, base, op)
}

