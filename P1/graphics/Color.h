#pragma once
#include <type_traits>

namespace P1 { namespace graphics {
	template <typename T>
	class SolidColor {
	private:
		T rgb[3] = { 0, 0, 0 };
	public:
		SolidColor() = default;
		SolidColor(T R, T G, T B) {
			int max = std::is_integral<T>::value ? 255 : 1;
			rgb[0] = R;
			rgb[1] = G;
			rgb[2] = B;
		}

		inline T r() { return rgb[0]; }
		inline T g() { return rgb[1]; }
		inline T b() { return rgb[2]; }

		inline T* data_ptr() { return &rgb[0]; }

		friend std::ostream& operator << (std::ostream& os, const SolidColor& col) {
			return os << "rgb(" << col.m_r << ", " << col.m_g << ", " << col.m_b << ')';
		}
	};
}}
