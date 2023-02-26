#pragma once

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Matrix.h"
#include "../miscellaneous/Proxy.h"
#include "../concepts/MathConcepts.h"

namespace P1::math {
	template <P1::concepts::Number T, unsigned int W, unsigned int H> class Matrix;

	template <P1::concepts::Number T = double>
	class Vector2: public Matrix<T, 1, 3> {
	public:
		unsigned int width() = delete;
		unsigned int height() = delete;

		Proxy<T> x;
		Proxy<T> y;

		Vector2(): Matrix<T, 1, 3>() {
			this->data[2][0] = 1;

			Proxy<T>::set_ref(x, this->data[0][0]);
			Proxy<T>::set_ref(y, this->data[1][0]);
		}
		Vector2(T x, T y): Matrix<T, 1, 3>() {
			this->data[0][0] = x;
			this->data[1][0] = y;
			this->data[2][0] = 1;

			Proxy<T>::set_ref(this->x, this->data[0][0]);
			Proxy<T>::set_ref(this->y, this->data[1][0]);
		}
		Vector2(const Vector2& other) : Matrix<T, 1, 3>() {
			this->data[0][0] = static_cast<T>(other.data[0][0]);
			this->data[1][0] = static_cast<T>(other.data[1][0]);
			this->data[2][0] = 1;

			Proxy<T>::set_ref(x, this->data[0][0]);
			Proxy<T>::set_ref(y, this->data[1][0]);
		}

		double& operator () (int x, int y) = delete;
		double operator () (int x, int y) const = delete;

		static Vector2<T> zero() { return Vector2<T>(); }
		static Vector2<T> one() { return Vector2<T>(1, 1); }

		Matrix<T, 3> to_scale_matrix() {
			return Matrix<T, 3>::scale(this->data[0][0], this->data[0][1]);
		}
		template <unsigned int M = 0>
		Matrix<T, 3> to_translate_matrix() {
			return Matrix<T, 3>::template translate<M>(this->data[0][0], this->data[0][1]);
		}

		template <typename _T>
		operator Vector2<_T> () {
			_T a = static_cast<_T>(x);
			_T b = static_cast<_T>(y);
			return Vector2<_T>(a, b);
		}

		double get_magnitude() {
			return hypot(x, y);
		}

		double get_radians() {
			return atan2(x, y);
		}
		double get_degrees() {
			return 180 * get_radians() / M_PI;
		}

		Vector2 normalized(double targetMagnitude = 1) {
			if(get_magnitude() != 0) {
				double mod = targetMagnitude / get_magnitude();
				return Vector2(x * mod, y * mod);
			}
			return Vector2();
		}
		Vector2 clamped(double targetMagnitude = 1) {
			if(get_magnitude() > targetMagnitude)
				return normalized(targetMagnitude);
			return Vector2(*this);
		}

		template <P1::concepts::Number _T>
		Vector2 operator + (const Vector2<_T>& other) {
			return Vector2(x + other.x, y + other.y);
		}
		inline Vector2 operator + () { return Vector2(+x, +y); }

		template <P1::concepts::Number _T>
		Vector2 operator += (const Vector2<_T>& other) {
			x += other.x;
			y += other.y;
			return *this;
		}

		template <P1::concepts::Number _T>
		Vector2 operator - (const Vector2<_T>& other) {
			return Vector2(x - other.x, y - other.y);
		}
		inline Vector2 operator - () { return Vector2(-x, -y); }

		template <P1::concepts::Number _T>
		Vector2 operator -= (const Vector2<_T>& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		template <P1::concepts::Number _T>
		double operator * (const Vector2<_T>& other) {
			return x * other.x + y * other.y;
		}

		template <P1::concepts::Number _T>
		Vector2 operator * (const _T& value) {
			return Vector2(x * value, y * value);
		}

		template <P1::concepts::Number _T>
		Vector2 operator / (const _T& value) {
			return Vector2(x / value, y / value);
		}

		friend Vector2 operator / (const T& value, const Vector2& v2) {
			return Vector2(value / v2.data[0][0], value / v2.data[0][1]);
		}

		template <P1::concepts::Number _T>
		Vector2 operator /= (const _T& value) {
			x /= value;
			y /= value;
			return *this;
		}

		template <P1::concepts::Number _T>
		Vector2 operator *= (const _T& value) {
			x *= value;
			y *= value;
			return *this;
		}

		Vector2& operator = (const Vector2& other) = default;
		Vector2& operator = (Vector2&& other) = default;

		friend std::ostream& operator << (std::ostream& os, const Vector2& v2) {
			return os << '(' << v2.x << ", " << v2.y << ')';
		}
	};
}
