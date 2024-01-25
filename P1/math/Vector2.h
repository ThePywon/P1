#pragma once

#include <ostream>
#include <type_traits>
#include <cmath>

namespace P1::math {
  template<typename T, typename = std::enable_if_t<std::is_arithmetic<T>::value>>
  class Vector2 {
  public:
    T x, y;
  
    inline Vector2(T x = 0, T y = 0) {
      this->x = x;
      this->y = y;
    }

    static inline Vector2 zero() {
      return Vector2{};
    }
    static inline Vector2 one() {
      return Vector2{1, 1};
    }
    static inline Vector2 right() {
      return Vector2{1, 0};
    }
    static inline Vector2 up() {
      return Vector2{0, 1};
    }

    static double distance(const Vector2& lhs, const Vector2& rhs) {
      double xdiff = (double)lhs.x - rhs.x;
      double ydiff = (double)lhs.y - rhs.y;

      return sqrt(xdiff * xdiff + ydiff * ydiff);
    }
    static double dot(const Vector2& lhs, const Vector2& rhs);

    double magnitude() {
      return sqrt(x * x + y * y);
    }
    Vector2 normalized() {
      double m = magnitude();
      double x = 0;
      double y = 0;

      if(this->x != 0) x = this->x / m;
      if(this->y != 0) y = this->y / m;

      return Vector2{ x, y };
    }

    friend std::ostream& operator << (std::ostream& os, const Vector2& v) {
      return os << "(" << v.x << ", " << v.y << ")";
    }
    friend Vector2 operator + (Vector2 lhs, const Vector2& rhs) {
      lhs.x += rhs.x;
      lhs.y += rhs.y;
      return lhs;
    }
    friend Vector2 operator - (Vector2 lhs, const Vector2& rhs) {
      lhs.x -= rhs.x;
      lhs.y -= rhs.y;
      return lhs;
    }
    friend Vector2 operator * (Vector2 lhs, const Vector2& rhs) {
      lhs.x *= rhs.x;
      lhs.y *= rhs.y;
      return lhs;
    }
    friend Vector2 operator / (Vector2 lhs, const Vector2& rhs) {
      lhs.x /= rhs.x;
      lhs.y /= rhs.y;
      return lhs;
    }
    Vector2& operator += (Vector2 rhs) {
      this->x += rhs.x;
      this->y += rhs.y;
      return *this;
    }
    Vector2& operator -= (Vector2 rhs) {
      this->x -= rhs.x;
      this->y -= rhs.y;
      return *this;
    }
    Vector2& operator *= (Vector2 rhs) {
      this->x *= rhs.x;
      this->y *= rhs.y;
      return *this;
    }
    Vector2& operator /= (Vector2 rhs) {
      this->x /= rhs.x;
      this->y /= rhs.y;
      return *this;
    }
    Vector2& operator = (Vector2 rhs) {
      this->x = rhs.x;
      this->y = rhs.y;
      return *this;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    friend Vector2 operator + (Vector2 lhs, const N& rhs) {
      lhs.x += rhs;
      lhs.y += rhs;
      return lhs;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    friend Vector2 operator - (Vector2 lhs, const N& rhs) {
      lhs.x -= rhs;
      lhs.y -= rhs;
      return lhs;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    friend Vector2 operator * (Vector2 lhs, const N& rhs) {
      lhs.x *= rhs;
      lhs.y *= rhs;
      return lhs;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    friend Vector2 operator / (Vector2 lhs, const N& rhs) {
      lhs.x /= rhs;
      lhs.y /= rhs;
      return lhs;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    Vector2& operator += (N rhs) {
      this->x += rhs;
      this->y += rhs;
      return *this;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    Vector2& operator -= (N rhs) {
      this->x -= rhs;
      this->y -= rhs;
      return *this;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    Vector2& operator *= (N rhs) {
      this->x *= rhs;
      this->y *= rhs;
      return *this;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    Vector2& operator /= (N rhs) {
      this->x /= rhs;
      this->y /= rhs;
      return *this;
    }
    template<typename N, typename = std::enable_if_t<std::is_arithmetic<N>::value>>
    operator Vector2<N>() const {
      return Vector2<N>{ static_cast<N>(this->x), static_cast<N>(this->y) };
    }
  };
  
  typedef Vector2<int> vector2Int;
  typedef Vector2<double> vector2;
}
