#pragma once

#include <type_traits>

template <class T, class = void>
struct is_complete : std::false_type
{};

template <class T>
struct is_complete< T, decltype(void(sizeof(T))) > : std::true_type
{};
