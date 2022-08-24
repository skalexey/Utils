#pragma once
#include "vector.h"

namespace math
{
	// Use CRTP pattern
	template <typename T>
	struct vector_2d : public vector<T, 2, vector_2d<T>>
	{
		using type = vector_2d<T>;
		using base = vector<T, 2, type>;

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
		
		vector_2d& operator=(const vector_2d& r) { return base::operator=(r); }
	};

	using vector2 = vector_2d<float>;
	using point2 = vector2;
}

