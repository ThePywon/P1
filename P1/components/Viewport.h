#pragma once

#include "../math/Rect.h"
#include "../components/Component.h"

namespace P1::components {
	struct Viewport : Component {
		P1::math::Rect<float> viewport;
	};
}
