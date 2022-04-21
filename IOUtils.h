//
//  IOUtils.h
//
//  Created by Alex Skorokhodov on 2022/04/21.
//

#pragma once

#include <iostream>
#include <iterator>

namespace Utils
{
	template <typename T>
	void PrintList(T& l)
	{
		std::cout << "[";
		for (auto it = l.begin();; ++it)
		{
			std::cout << *it;
			if (it == std::prev(l.end()))
			{
				std::cout << "]";
				break;
			}
			else
			{
				std::cout << ", ";
			}
		}
	}
}

