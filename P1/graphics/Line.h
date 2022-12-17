#pragma once
#include <vector>
#include "../math/Vector2.h"
#include "../graphics/Color.h"

namespace P1 { namespace graphics {
	template <typename T>
	class Line {
	public:
		std::vector<P1::math::Vector2<T>> points;
		P1::graphics::SolidColor<float> color;

		double pointSize;
		double lineWidth;

		inline Line(std::vector<P1::math::Vector2<T>> points,
			P1::graphics::SolidColor<float> color, double pointSize = 10, double lineWidth = 2.5) {
			this->points = points;
			this->color = color;
			this->pointSize = pointSize;
			this->lineWidth = lineWidth;
		}
		inline Line(P1::math::Vector2<T> A, P1::math::Vector2<T> B,
			P1::graphics::SolidColor<float> color, double pointSize = 10, double lineWidth = 2.5) {
			points = std::vector<P1::math::Vector2<T>>{A, B};
			this->color = color;
			this->pointSize = pointSize;
			this->lineWidth = lineWidth;
		}

		void render() {
			glPointSize(pointSize);
			glLineWidth(lineWidth);
			glColor3f(color.r(), color.g(), color.b());
			glBegin(GL_LINES);
			for(typename std::vector<P1::math::Vector2<T>>::iterator v = points.begin(); v != points.end(); ++v)
				glVertex2f(v->x, v->y);
			glEnd();
		}
	};
}}
