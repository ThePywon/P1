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
	class Vector3 : public Matrix<T, 1, 4> {
	public:
		unsigned int width() = delete;
		unsigned int height() = delete;
		
		Proxy<T> x{};
		Proxy<T> y{};
		Proxy<T> z{};

		Vector3(): Matrix<T, 1, 4>() {
			this->data[0][0] = 0;
			this->data[1][0] = 0;
			this->data[2][0] = 0;
			this->data[3][0] = 1;

			Proxy<T>::set_ref(x, this->data[0][0]);
			Proxy<T>::set_ref(y, this->data[1][0]);
			Proxy<T>::set_ref(z, this->data[2][0]);
		}
		Vector3(T x, T y, T z): Matrix<T, 1, 4>() {
			this->data[0][0] = x;
			this->data[1][0] = y;
			this->data[2][0] = z;
			this->data[3][0] = 1;

			Proxy<T>::set_ref(this->x, this->data[0][0]);
			Proxy<T>::set_ref(this->y, this->data[1][0]);
			Proxy<T>::set_ref(this->z, this->data[2][0]);
		}
		template <P1::concepts::Number _T>
		Vector3(const Vector3<_T>& other) : Matrix<T, 1, 4>() {
			this->data[0][0] = static_cast<T>(other.data[0][0]);
			this->data[1][0] = static_cast<T>(other.data[1][0]);
			this->data[2][0] = static_cast<T>(other.data[2][0]);
			this->data[3][0] = 1;

			Proxy<T>::set_ref(x, this->data[0][0]);
			Proxy<T>::set_ref(y, this->data[1][0]);
			Proxy<T>::set_ref(z, this->data[2][0]);
		}

		static Vector3<T> zero() { return Vector3<T>(); }
		static Vector3<T> one() { return Vector3<T>(1, 1, 1); }

		Matrix<T, 4, 4> to_scale_matrix() {
			return Matrix<T, 4, 4>::scale(x, y, z);
		}
		Matrix<T, 4, 4> to_translate_matrix() {
			return Matrix<T, 4, 4>::translate(x, y, z);
		}

		double get_magnitude() {
			return hypot(hypot(x, y), z);
		}

		Vector3 normalized(double targetMagnitude = 1) {
			if(get_magnitude() != 0) {
				double mod = targetMagnitude / get_magnitude();
				return Vector3(x * mod, y * mod, z * mod);
			}
			return Vector3();
		}
		Vector3 clamped(double targetMagnitude = 1) {
			if(get_magnitude() > targetMagnitude)
				return normalized(targetMagnitude);
			return Vector3<T>(*this);
		}

		template <P1::concepts::Number _T>
		Vector3 cross(const Vector3<_T>& other) {
			return Vector3(y * other.z - z * other.y,
				z * other.x - x * other.z, x * other.y - y * other.x);
		}

		template <P1::concepts::Number _T>
		Vector3 operator + (const Vector3<_T>& other) {
			return Vector3(x + other.x, y + other.y, z + other.z);
		}
		inline Vector3 operator + () { return Vector3(+x, +y, +z); }

		template <P1::concepts::Number _T>
		Vector3 operator += (const Vector3<_T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		template <P1::concepts::Number _T>
		Vector3 operator - (const Vector3<_T>& other) {
			return Vector3(x - other.x, y - other.y, z - other.z);
		}
		inline Vector3 operator - () { return Vector3(-x, -y, -z); }

		template <P1::concepts::Number _T>
		Vector3 operator -= (const Vector3<_T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}

		template <P1::concepts::Number _T>
		double operator * (const Vector3<_T>& other) {
			return x * other.x + y * other.y + z * other.z;
		}

		template <P1::concepts::Number _T>
		Vector3 operator /= (const _T& value) {
			x /= value;
			y /= value;
			z /= value;
			return *this;
		}

		template <P1::concepts::Number _T>
		Vector3 operator / (const _T& value) {
			return Vector3(x / value, y / value, z / value);
		}

		template <P1::concepts::Number _T>
		Vector3 operator *= (const _T& value) {
			x *= value;
			y *= value;
			z *= value;
			return *this;
		}

		template <P1::concepts::Number _T>
		Vector3 operator * (const _T& value) {
			return Vector3(x * value, y * value, z * value);
		}
		Vector3& operator = (Vector3&& other) = default;

		friend std::ostream& operator << (std::ostream& os, const Vector3& v3) {
			return os << '(' << v3.x << ", " << v3.y << ", " << v3.z << ')';
		}
	};
}
