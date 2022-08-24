#pragma once
#include "vector.h"

namespace math
{
	// Use CRTP pattern
	template <typename T>
	struct vector_3d : public vector<T, 3, vector_3d<T>>
	{
		using type = vector_3d<T>;
		using base = vector<T, 3, type>;

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

		vector_3d& operator=(const vector_3d& r) { return base::operator=(r); }
	};

	using vector3 = vector_3d<float>;
	using point3 = vector3;
}

