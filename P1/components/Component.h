#pragma once

#include <vector>
#include <memory>
#include "../concepts/GameConcepts.h"

namespace P1::entity { class Scene; class Entity; }

namespace P1::components { 
	struct Component {
	private:
		// Store entity id and type id
		friend entity::Scene;
		friend entity::Entity;
		unsigned int entity_id;
		unsigned int id;
	public:
		// Make polymorphic
		virtual ~Component() {};
	};

	extern unsigned int component_id_counter;
	template <concepts::Component T>
	unsigned int get_component_id() {
		// Let the compiler generate a different static id for every component type
		static unsigned int id = component_id_counter++;
		return id;
	}

	class ComponentPoolBase {
	public:
		// Make polymorphic
		virtual ~ComponentPoolBase() {};
	};

	template <concepts::Component T>
	class ComponentPool : public ComponentPoolBase {
	public:
		// Store components
		std::vector<std::shared_ptr<T>> components{};
	};
}
