#pragma once

#include "../concepts/MathConcepts.h"

namespace P1::math {
	template <typename T>
	requires P1::concepts::Number<T>
	class Rect {
	public:
		T left = 0;
		T top = 0;
		T width = 0;
		T height = 0;

		Rect() = default;
		Rect(T left, T top, T width, T height) {
			this->left = left;
			this->top = top;
			this->width = width;
			this->height = height;
		}
	};
}
