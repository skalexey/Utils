#pragma once

namespace math
{
	template <typename T>
	struct rect
	{
		rect() = default;
		rect(T x, T y, T w, T h)
			: x(x), y(y), w(w), h(h)
		{}
		T x = 0.f;
		T y = 0.f;
		T w = 0.f;
		T h = 0.f;
	};
}

using Rect = math::rect<float>;