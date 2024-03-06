#pragma once
#include "vector.h"

namespace math
{
	// CRTP pattern
	template <typename T>
	struct vector_4d : public vector<T, 4, vector_4d<T>>
	{
		using type = vector_4d<T>;
		using base = vector<T, 4, type>;

		vector_4d()
			: base()
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
			, w(base::data(3))
		{}
		vector_4d(const vector_4d& r)
			: base(r)
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
			, w(base::data(3))
		{}
		vector_4d(T x, T y, T z, T w)
			: base()
			, x(base::data(0))
			, y(base::data(1))
			, z(base::data(2))
			, w(base::data(3))
		{
			base::data(0) = x;
			base::data(1) = y;
			base::data(2) = z;
			base::data(3) = w;
		}
		T& x;
		T& y;
		T& z;
		T& w;

		vector_4d& operator=(const vector_4d& r) { return base::operator=(r); }
	};

	using vector4 = vector_4d<float>;
	using point4 = vector4;
}

