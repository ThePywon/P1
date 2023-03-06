#pragma once

#include <iostream>
#include <memory>
#include "../concepts/GameConcepts.h"
#include "../events/EventManager.h"
#include "Component.h"

namespace P1::entity { class Entity; }

namespace P1::components {
	#define SIGNATURE_CHANGE_EVENT 0
	#define GARBAGE_COLLECTION_EVENT 1

	class GarbageCollectorBase {
	public:
		virtual void collect(unsigned int entity_id) { };
	};

	extern unsigned int get_entity_id(entity::Entity* entity);

	template <concepts::Component T>
	class GarbageCollector : public GarbageCollectorBase {
	private:
		std::shared_ptr<components::ComponentPool<T>> component_pool;
	public:
		GarbageCollector(std::shared_ptr<components::ComponentPool<T>> pool) : GarbageCollectorBase() {
			component_pool = pool;
		}

		void collect(unsigned int entity_id) {
			for(auto it = component_pool->components.begin(); it != component_pool->components.end(); ++it) {
				if((*it)->get_entity_id() != entity_id) continue;

				std::cout << "Collected component #" << get_component_id<T>() << " for entity #" << entity_id << std::endl;

				component_pool->components.erase(it);
				return;
			}
		}
	};
}
