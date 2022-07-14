﻿// pair_hash.h

#pragma once

#include <functional>

// Define hash for std::pair
namespace std {
	template<typename X, typename Y>
	struct hash<std::pair<X, Y>> {
		std::size_t operator()(const std::pair<X, Y>& pair) const {
			return std::hash<X>()(pair.first) ^ std::hash<Y>()(pair.second);
		}
	};
}
