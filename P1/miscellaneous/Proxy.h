#pragma once

#include "../concepts/MathConcepts.h"

template <typename T>
class Proxy {
private:
	T* value;
public:
	Proxy() : value(NULL) { };
	Proxy(T& v) : value(&v) { }

	static inline void set_ref(Proxy<T>& p, T& v) { p.value = &v; }

	static std::string to_string(const Proxy& proxy) {
		return std::to_string(proxy->value);
	}

	template <typename _T>
	operator _T () { return static_cast<_T>(*value); }
	T operator * () { return *value; }

	template <typename _T>
	T operator = (const _T& v) { return *value = static_cast<T>(v); }
	friend std::ostream& operator << (std::ostream& os, const Proxy& p) { return os << *p.value; }
	template <typename _T>
	T operator + (const _T& v) { return *value + v; }
	template <typename _T>
	T operator + (const Proxy<_T>& p) { return *value + (*p.value); }
	template <typename _T>
	friend _T operator + (const _T& v, const Proxy<T>& p) { return v + *p.value; }
	template <typename _T>
	T operator - (const _T& v) { return *value - v; }
	template <typename _T>
	T operator - (const Proxy<_T>& p) { return *value - (*p.value); }
	template <typename _T>
	friend _T operator - (const _T& v, const Proxy<T>& p) { return v - *p.value; }
	template <typename _T>
	T operator * (const _T& v) { return *value * v; }
	template <typename _T>
	T operator * (const Proxy<_T>& p) { return *value * (*p.value); }
	template <typename _T>
	friend _T operator * (const _T& v, const Proxy<T>& p) { return v * *p.value; }
	template <typename _T>
	T operator / (const _T& v) { return *value / v; }
	template <typename _T>
	T operator / (const Proxy<_T>& p) { return *value / (*p.value); }
	template <typename _T>
	friend _T operator / (const _T& v, const Proxy<T>& p) { return v / *p.value; }
	template <typename _T>
	T operator += (const _T& v) { return *value += v; }
	template <typename _T>
	T operator += (const Proxy<_T>& p) { return *value += (*p.value); }
	template <typename _T>
	T operator -= (const _T& v) { return *value -= v; }
	template <typename _T>
	T operator -= (const Proxy<_T>& p) { return *value -= (*p.value); }
	template <typename _T>
	T operator *= (const _T& v) { return *value *= v; }
	template <typename _T>
	T operator *= (const Proxy<_T>& p) { return *value *= (*p.value); }
	template <typename _T>
	T operator /= (const _T& v) { return *value /= v; }
	template <typename _T>
	T operator /= (const Proxy<_T>& p) { return *value /= (*p.value); }
	T operator + () { return +*value; }
	T operator - () { return -*value; }

	std::string to_string() { return std::to_string(*value); }
};
