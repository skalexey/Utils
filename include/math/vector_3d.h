#pragma once
#include "vector.h"

namespace math
{
	template <typename T>
	struct vector_3d : public vector<T, 3>
	{
		using base = vector<T, 3>;
		using type = vector_3d<T>;

		vector_3d()
			: base()
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
		{}
		vector_3d(const vector_2d& r)
			: base(r)
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
		{}
		vector_3d(T x, T y, T z)
			: base()
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
		{
			base::data(0) = x;
			base::data(1) = y;
			base::data(2) = z;
		}
		T& x;
		T& y;
		T& z;

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
		type operator * (const R& s) const { return type(*this) *= T(s); }
		template <typename R>
		type operator / (const R& s) const { return type(*this) /= T(s); }
	};

	using vector3 = vector_3d<float>;
	using point3 = vector3;
}

