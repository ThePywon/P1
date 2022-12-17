#pragma once

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <assert.h>
#include "Concepts.h"
#include "Vector2.h"
#include "Vector3.h"

namespace P1::math {
	template <typename T> class Vector2;
	template <typename T> class Vector3;

	template <typename T>
	class Matrix {
	protected:
		unsigned int m_width, m_height;

		std::vector<std::vector<T>> data;
	public:
		inline unsigned int width() { return m_width; }
		inline unsigned int height() { return m_height; }

		Matrix(unsigned int width) {
			assert(width > 0);

			m_width = width;
			m_height = width;

			for(int y = 0; y < width; y++) {
				std::vector<T> result;
				for(int x = 0; x < width; x++)
					result.push_back(0);
				data.push_back(result);
			}
		}
		Matrix(unsigned int width, unsigned int height) {
			assert(width > 0);
			assert(height > 0);

			m_width = width;
			m_height = height;

			for(int y = 0; y < height; y++) {
				std::vector<T> result;
				for(int x = 0; x < width; x++)
					result.push_back(0);
				data.push_back(result);
			}
		}

		static Matrix<T> identity(unsigned int width) {
			assert(width > 0);

			Matrix<T> result(width, width);
			for(int i = 0; i < width; i++)
				result.data[i][i] = 1;
			return result;
		}

		static Matrix<T> scale(unsigned int width, std::initializer_list<T> args) {
			assert(args.size() + 1 == width);

			Matrix<T> result(width, width);
			int index = 0;
			for(auto arg : args) {
				result.data[index][index] = arg;
				index++;
			}
			result.data[width-1][width-1] = 1;
			return result;
		}

		static Matrix<T> translate(unsigned int width, std::initializer_list<T> args) {
			assert(args.size() + 1 == width);

			Matrix<T> result(width, width);
			int index = 0;
			for(auto arg : args) {
				result.data[index][width-1] = arg;
				index++;
			}
			for(int i = 0; i < width; i++)
				result.data[i][i] = 1;
			return result;
		}

		double& operator () (unsigned int x, unsigned int y) {
			assert(x < m_width);
			assert(y < m_height);

			return data[y][x];
		}
		double operator () (unsigned int x, unsigned int y) const {
			assert(x < m_width);
			assert(y < m_height);

			return data[y][x];
		}

		template <typename _T>
		Matrix operator + (const Matrix<_T>& other) {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = this->data[y][x] + other.data[y][x];
			return result;
		}

		Matrix operator + () {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = +this->data[y][x];
			return result;
		}

		template <typename _T>
		Matrix operator += (const Matrix<_T>& other) {
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					this->data[y][x] += other.data[y][x];
			return *this;
		}

		template <typename _T>
		Matrix operator - (const Matrix<_T>& other) {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = this->data[y][x] - other.data[y][x];
			return result;
		}

		Matrix operator - () {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = -this->data[y][x];
			return result;
		}

		template <typename _T>
		Matrix& operator -= (const Matrix<_T>& other) {
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					this->data[y][x] -= other.data[y][x];
			return *this;
		}

		template <typename _T>
		requires P1::concepts::Number<_T>
		Matrix operator * (const _T& value) {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = this->data[y][x] * value;
			return result;
		}
		template <typename _T>
		friend Matrix operator * (const _T& value, const Matrix& mat) {
			Matrix result(mat.m_width, mat.m_height);
			for(int y = 0; y < mat.m_height; y++)
				for(int x = 0; x < mat.m_width; x++)
					result.data[y][x] = mat.data[y][x] * value;
			return result;
		}

		template <typename _T>
		Matrix operator * (const Matrix<_T>& other) {
			assert(m_width == other.m_height);

			Matrix result(other.m_width, m_height);
			for(int y = 0; y < other.m_height; y++) {
				for(int x = 0; x < other.m_width; x++) {
					T value = 0;
					for(int i = 0; i < m_width; i++)
						value += this->data[y][i] * other.data[i][x];
					result.data[y][x] = value;
				}
			}

			return result;
		}

		template <typename _T>
		requires P1::concepts::Number<_T>
		Matrix& operator *= (const _T& value) {
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					this->data[y][x] *= value;
			return *this;
		}

		template <typename _T>
		Matrix& operator *= (const Matrix<_T>& other) {
			assert(other.m_width == m_height);

			Matrix result(m_width, other.m_height);
			for(int y = 0; y < m_height; y++) {
				for(int x = 0; x < m_width; x++) {
					T value = 0;
					for(int i = 0; i < other.m_width; i++)
						value += other.data[y][i] * this->data[i][x];
					result.data[y][x] = value;
				}
			}
			this->m_width = result.m_width;
			this->m_height = result.m_height;
			this->data = result.data;

			return *this;
		}

		template <typename _T>
		requires P1::concepts::Number<_T>
		Matrix operator / (const T& value) {
			Matrix result(m_width, m_height);
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					result.data[y][x] = this->data[y][x] / value;
			return result;
		}

		template <typename _T>
		requires P1::concepts::Number<_T>
		Matrix& operator /= (const _T& value) {
			for(int y = 0; y < m_height; y++)
				for(int x = 0; x < m_width; x++)
					this->data[y][x] /= value;
			return *this;
		}

		operator Vector2<T> () {
			assert(m_width == 1);
			assert(m_height == 3);

			return Vector2(data[0][0], data[1][0]);
		}

		operator Vector3<T> () {
			assert(m_width == 1);
			assert(m_height == 4);

			return Vector3(data[0][0], data[1][0], data[2][0]);
		}

		template <typename _T>
		Matrix operator = (const Matrix<_T>& other) {
			data = other.data;
			m_width = other.m_width;
			m_height = other.m_height;

			return *this;
		}

		static std::string to_string(const Matrix& mat) {
			std::string result;
			for(int y = 0; y < mat.data.size(); y++) {
				result += y > 0 ? "\n[" : "[";
				for(int x = 0; x < mat.data[0].size(); x++)
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
