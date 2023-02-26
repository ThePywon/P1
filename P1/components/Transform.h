#pragma once

#include "../math/Vector3.h"
#include "../math/Matrix.h"
#include "../components/Component.h"

namespace P1::components {
	template <P1::concepts::Number T = float>
	struct Transform : Component {
		math::Vector3<T> position{};
		math::Vector3<T> scale = math::Vector3<T>::one();
	};
}
