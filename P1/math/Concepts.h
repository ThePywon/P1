#pragma once

#include <concepts>

namespace P1 { namespace concepts {
	template <typename T>
	concept Number = std::integral<T> || std::floating_point<T>;
}}
