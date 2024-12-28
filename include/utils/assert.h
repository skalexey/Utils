#pragma once

#include <type_traits>

#define ASSERT_NONCONST(var) static_assert(!std::is_const<std::remove_reference_t<decltype(var)>>::value, "Variable '" #var "' must be non-const. Please ensure you are not trying to modify a const object.")
