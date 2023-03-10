#pragma once

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <assert.h>
#include "../concepts/MathConcepts.h"
#include "Vector2.h"
#include "Vector3.h"

namespace P1::math {
	template <P1::concepts::Number T> class Vector2;
	template <P1::concepts::Number T> class Vector3;

	#define ROW_MAJOR 0
	#define COLUMN_MAJOR 1

	template <P1::concepts::Number T, unsigned int W, unsigned int H = W>
	class Matrix {
	public:
		T data[H][W];

		static constexpr unsigned int width = W;
		static constexpr unsigned int height = H;

		Matrix() {
			static_assert(W != 0 && H != 0);

			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					data[y][x] = 0;
		}

		template <concepts::IS<T>... Args>
		Matrix(Args... args) {
			static_assert(W != 0 && H != 0);
			static_assert(sizeof...(args) == W * H);

			int index = 0;
			T values[] = { static_cast<T>(args)... };

			for(auto arg : values) {
				data[index / W][index % W] = arg;
				index++;
			}
		}

		Matrix(T args[H][W]) {
			static_assert(W != 0 && H != 0);

			data = args;
		}

		static constexpr bool is_square = W == H;

		static Matrix identity() {
			static_assert(W != 0 && H != 0 && W == H);

			Matrix result{};
			for(int i = 0; i < W; i++)
				result.data[i][i] = 1;
			return result;
		}

		template <concepts::IS<T>... Args>
		requires(W != 0 && H != 0 && W == H)
		static Matrix scale(Args... args) {
			static_assert(sizeof...(args) + 1 == W);

			Matrix result{};
			int index = 0;
			T values[] = { static_cast<T>(args)... };

			for(T arg : values) {
				result.data[index][index] = arg;
				index++;
			}
			result.data[H-1][W-1] = 1;

			return result;
		}

		static Matrix scale(Vector2<T> v2) {
			static_assert(W == 3 && H == 3);
			return Matrix{ *v2.x, (T)0, (T)0,
										 (T)0, *v2.y, (T)0,
										 (T)0, (T)0, (T)1 };
		}

		static Matrix scale(Vector3<T> v3) {
			static_assert(W == 4 && H == 4);
			return Matrix{ *v3.x, (T)0, (T)0, (T)0,
										 (T)0, *v3.y, (T)0, (T)0,
										 (T)0, (T)0, *v3.z, (T)0,
										 (T)0, (T)0, (T)0, (T)1 };
		}

		template <unsigned int M = ROW_MAJOR, concepts::IS<T>... Args>
		requires(W != 0 && H != 0 && W == H)
		static Matrix translate(Args... args) {
			static_assert(sizeof...(args) + 1 == W);

			Matrix result{};
			int index = 0;
			T values[] = { static_cast<T>(args)... };

			if(M == ROW_MAJOR) {
				for(auto arg : values) {
					result.data[index][W-1] = arg;
					index++;
				}
			}
			else {
				for(auto arg : values) {
					result.data[H-1][index] = arg;
					index++;
				}
			}
			for(int i = 0; i < W; i++)
				result.data[i][i] = 1;

			return result;
		}

		template <unsigned int M = ROW_MAJOR>
		requires(W == 3 && H == 3)
		static Matrix translate(Vector2<T> v2) {
			if(M == ROW_MAJOR)
				return Matrix{ (T)1, (T)0, *v2.x,
											 (T)0, (T)1, *v2.y,
											 (T)0, (T)0, (T)1 };

			return Matrix{ (T)1, (T)0, (T)0,
										 (T)0, (T)1, (T)0,
										 *v2.x, *v2.y, (T)1 };
		}

		template <unsigned int M = ROW_MAJOR>
		requires(W == 4 && H == 4)
		static Matrix translate(Vector3<T> v3) {
			if(M == ROW_MAJOR)
				return Matrix{ (T)1, (T)0, (T)0, *v3.x,
											 (T)0, (T)1, (T)0, *v3.y,
											 (T)0, (T)0, (T)1, *v3.z,
											 (T)0, (T)0, (T)0, (T)1 };

			return Matrix{ (T)1, (T)0, (T)0, (T)0,
										 (T)0, (T)1, (T)0, (T)0,
										 (T)0, (T)0, (T)1, (T)0,
										 *v3.x, *v3.y, *v3.z, (T)1 };
		}

		double& operator () (unsigned int x, unsigned int y) {
			assert(x < W && y < H);

			return data[y][x];
		}
		double operator () (unsigned int x, unsigned int y) const {
			assert(x < W && y < H);

			return data[y][x];
		}

		Matrix<T, H, W> transpose() {
			Matrix<T, H, W> result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[x][y] = this->data[y][x];
			return result;
		}

		template <P1::concepts::Number _T>
		Matrix operator + (const Matrix<_T, W, H>& other) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = this->data[y][x] + other.data[y][x];
			return result;
		}

		Matrix operator + () {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = +this->data[y][x];
			return result;
		}

		template <P1::concepts::Number _T>
		Matrix operator += (const Matrix<_T, W, H>& other) {
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					this->data[y][x] += other.data[y][x];
			return *this;
		}

		template <P1::concepts::Number _T>
		Matrix operator - (const Matrix<_T, W, H>& other) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = this->data[y][x] - other.data[y][x];
			return result;
		}

		Matrix operator - () {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = -this->data[y][x];
			return result;
		}

		template <P1::concepts::Number _T>
		Matrix& operator -= (const Matrix<_T, W, H>& other) {
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					this->data[y][x] -= other.data[y][x];
			return *this;
		}

		template <P1::concepts::Number _T>
		Matrix operator * (const _T& value) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = this->data[y][x] * value;
			return result;
		}
		template <P1::concepts::Number _T>
		friend Matrix operator * (const _T& value, const Matrix& mat) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = mat.data[y][x] * value;
			return result;
		}

		template <concepts::Number _T, unsigned int _W>
		Matrix<T, _W, H> operator * (const Matrix<_T, _W, W>& other) {
			Matrix<T, _W, H> result{};

			for(int y = 0; y < H; y++) {
				for(int x = 0; x < _W; x++) {
					T value = 0;
					for(int i = 0; i < W; i++)
						value += this->data[y][i] * other.data[i][x];
					result.data[y][x] = value;
				}
			}

			return result;
		}

		template <P1::concepts::Number _T>
		Matrix& operator *= (const _T& value) {
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					this->data[y][x] *= value;
			return *this;
		}

		template <concepts::Number _T>
		requires(W == H)
		Matrix& operator *= (const Matrix<_T, W>& other) {
			Matrix<_T, W> result{};

			for(int y = 0; y < H; y++) {
				for(int x = 0; x < W; x++) {
					T value = 0;
					for(int i = 0; i < W; i++)
						value += this->data[y][i] * other.data[i][x];
					result.data[y][x] = value;
				}
			}

			return result;
		}

		template <concepts::Number _T>
		Matrix operator / (const _T& value) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = this->data[y][x] / value;
			return result;
		}

		template <P1::concepts::Number _T>
		friend Matrix operator / (const _T& value, const Matrix& mat) {
			Matrix result{};
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					result.data[y][x] = value / mat.data[y][x];
			return result;
		}

		template <P1::concepts::Number _T>
		Matrix& operator /= (const _T& value) {
			for(int y = 0; y < H; y++)
				for(int x = 0; x < W; x++)
					this->data[y][x] /= value;
			return *this;
		}

		operator Vector2<T> () {
			static_assert(W == 1 && H == 3);

			return Vector2(data[0][0], data[1][0]);
		}

		operator Vector3<T> () {
			static_assert(W == 1 && H == 4);

			return Vector3(data[0][0], data[1][0], data[2][0]);
		}

		template <P1::concepts::Number _T>
		Matrix operator = (const Matrix<_T, W, H>& other) {
			data = other.data;
			return *this;
		}

		static std::string to_string(const Matrix& mat) {
			std::string result;
			for(int y = 0; y < H; y++) {
				result += y > 0 ? "\n[" : "[";
				for(int x = 0; x < W; x++)
					result += ' ' + std::to_string(mat.data[y][x]);
				result += " ]";
			}
			return result;
		}

		friend std::ostream& operator << (std::ostream& os, const Matrix& mat) {
			return os << Matrix::to_string(mat);
		}
	};
}
