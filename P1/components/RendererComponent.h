#pragma once

#include <vector>
#include "../components/Component.h"
#include "../graphics/Color.h"

namespace P1::components {
	struct RendererComponent : Component {
		unsigned int draw_mode = 4; // GL_TRIANGLES

		std::vector<float> vertices{};
		graphics::SolidColor<float> color{};

		float line_width = 5;
		float point_size = 5;
	};
}
