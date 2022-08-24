#pragma once
#include "vector.h"

namespace math
{
	template <typename T>
	struct vector_2d : public vector<T, 2>
	{
		using base = vector<T, 2>;
		using type = vector_2d<T>;

		vector_2d()
			: base()
			, x(base::data(0))
			, y(base::data(1))
		{}
		vector_2d(const vector_2d& r)
			: base(r) 
			, x(base::data(0))
			, y(base::data(1))
		{}
		vector_2d(T x, T y)
			: base()
			, x(base::data(0))
			, y(base::data(1))
		{
			base::data(0) = x;
			base::data(1) = y;
		}
		T& x;
		T& y;
		
		// Mirrorred operators
		VEC_OVERLOAD_REF(operator=)
		VEC_OVERLOAD_REF(operator+=)
		VEC_OVERLOAD_REF(operator-=)
		VEC_OVERLOAD_REF(operator*=)
		VEC_OVERLOAD_REF(operator/=)
		VEC_OVERLOAD_REF_CUSTOM(operator*=)
		VEC_OVERLOAD_REF_CUSTOM(operator/=)

		type operator + (const type& r) const { return type(*this) += r; }
		type operator - (const type& r) const { return type(*this) -= r; };
		type operator - () const { return type(*this) *= T(-1); }
		type operator * (const type& r) const { return type(*this) *= r; }
		type operator / (const type& r) const { return type(*this) /= r; }
		type normalized() const { return type(*this).normalize(); }
		template <typename R>
		type operator * (const R& r) const { return type(*this) *= T(r); }
		template <typename R>
		type operator / (const R& r) const { return type(*this) /= T(r); }
	};

	using vector2 = vector_2d<float>;
	using point2 = vector2;
}

