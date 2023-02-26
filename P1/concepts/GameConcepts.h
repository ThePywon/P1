#pragma once

#include <concepts>
#include <type_traits>

namespace P1::components { struct Component; }
namespace P1::systems { class SystemBase; }

namespace P1::concepts {
	template <typename T>
	concept Component = std::is_base_of<components::Component, T>::value;

	template <typename T>
	concept System = std::is_base_of<systems::SystemBase, T>::value;
}
