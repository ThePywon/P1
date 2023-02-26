#pragma once

#include <vector>
#include "../components/Component.h"
#include "../math/Vector2.h"
#include "../graphics/Color.h"

namespace P1::components {
	struct LineRendererComponent : Component {
		std::vector<float> vertices{
			-0.25, -0.25, 0,
			-0.25, 0.25, 0,
			-0.25, 0.25, 0,
			0.25, 0.25, 0,
			0.25, 0.25, 0,
			0.25, -0.25, 0,
			0.25, -0.25, 0,
			-0.25, -0.25, 0
		};
		P1::graphics::SolidColor<float> color{};

		float line_width = 5;
	};
}
