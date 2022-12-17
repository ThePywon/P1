#pragma once
#include <type_traits>

namespace P1 { namespace graphics {
	template <typename T>
	class SolidColor {
	private:
		T m_r = 0;
		T m_g = 0;
		T m_b = 0;
	public:
		SolidColor() = default;
		SolidColor(T R, T G, T B) {
			int max = std::is_integral<T>::value ? 255 : 1;
			m_r = R;
			m_g = G;
			m_b = B;

			if(m_r < 0) m_r = 0;
			else if(m_r > max) m_r = max;
			if(m_g < 0) m_g = 0;
			else if(m_g > max) m_g = max;
			if(m_b < 0) m_b = 0;
			else if(m_b > max) m_b = max;
		}

		inline T r() { return m_r; }
		inline T g() { return m_g; }
		inline T b() { return m_b; }

		friend std::ostream& operator << (std::ostream& os, const SolidColor& col) {
			return os << "rgb(" << col.m_r << ", " << col.m_g << ", " << col.m_b << ')';
		}
	};
}}
