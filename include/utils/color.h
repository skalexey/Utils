#pragma once

#include <limits>
#include <string>
#include <math/vector_2d.h>

namespace utils
{
	template <typename T>
	class color4 : public math::vector_4d<T>
	{
	public:
		static const T max;
		color4(const std::string& hex, T alpha = max) {
			if (hex.size() == 7 && hex[0] == '#') {
				auto r = std::stoi(hex.substr(1, 2), nullptr, 16);
				auto g = std::stoi(hex.substr(3, 2), nullptr, 16);
				auto b = std::stoi(hex.substr(5, 2), nullptr, 16);
				this->x = r / 255.f * max;
				this->y = g / 255.f * max;
				this->z = b / 255.f * max;
				this->w = alpha;
			}
		}
		std::string to_hex() const {
			auto r = int(float(this->x) / max * 255);
			auto g = int(float(this->y) / max * 255);
			auto b = int(float(this->z) / max * 255);
			return "#" + std::to_string(r) + std::to_string(g) + std::to_string(b);
		}
	};
	using color4f = color4<float>;
	using color4i = color4<unsigned char>;

	float color4f::max = 1.f;
	unsigned char color4i::max = 255;
}
