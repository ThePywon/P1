#pragma once

#include <concepts>

namespace P1::concepts {
	template <typename T>
	concept Number = std::integral<T> || std::floating_point<T>;

	template <typename T, typename U>
	concept IS = std::is_same<T, U>::value;
}
