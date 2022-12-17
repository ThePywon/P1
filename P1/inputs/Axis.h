#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>
#include "InputManager.h"
#include "../graphics/Window.h"
#include "../math/Vector2.h"

namespace P1::inputs {
	class Axis {
	private:
		friend void update(Axis* axis);
		friend void getRaw(Axis* axis);
		std::shared_ptr<InputListener> context;
		std::vector<int> positives;
		std::vector<int> negatives;

		int m_raw = 0;
		double m_linear = 0;
		double m_smooth = 0;

		void update_linear();
		void update_smooth();
	public:
		double speed = 0.1;
		double dead = 0.01;
		double smoothness = 5;

		Axis(P1::graphics::Window* window, std::vector<int> positives,
				std::vector<int> negatives);

		inline int raw() { return m_raw; }
		inline double linear() { return m_linear; }
		inline double smooth() { return m_smooth; }
	};

	class Axis2D {
	private:
		friend void update(Axis2D* axis);
		friend void getRaw(Axis2D* axis);

		std::unique_ptr<Axis> x;
		std::unique_ptr<Axis> y;
	public:
		Axis2D(P1::graphics::Window* window, std::vector<int> left, std::vector<int> up,
				std::vector<int> right, std::vector<int> down);

		inline P1::math::Vector2<double> raw() { return P1::math::Vector2<double>(x->raw(), y->raw()).clamped(); }
		inline P1::math::Vector2<double> linear() { return P1::math::Vector2<double>(x->linear(), y->linear()).clamped(); }
		inline P1::math::Vector2<double> smooth() { return P1::math::Vector2<double>(x->smooth(), y->smooth()).clamped(); }
	};
}
