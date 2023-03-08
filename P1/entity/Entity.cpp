#include "Entity.h"

namespace P1::entity {
	unsigned int entity_id_counter = 0;
	std::vector<unsigned int> freed_ids = {};
	unsigned int get_entity_id() {
		if(freed_ids.size()) {
			unsigned int id = freed_ids.back();
			freed_ids.pop_back();
			return id;
		}
		return entity_id_counter++;
	}

	Entity* Entity::create(Scene* scene, const std::string& name) {
		std::shared_ptr<Entity> entity = std::make_shared<Entity>(use_create_method{}, name, scene);
		scene->entities.push_back(entity);
		systems::MainManager::logger->debug("Entity \"" + name + "\" #" + std::to_string(entity->id) + " created.");
		return entity.get();
	}

	void Entity::destroy() {
		unsigned int old_mask = component_mask;

		component_mask = 0;
		scene->event_manager.emit(this);

		for(auto it = scene->entities.begin(); it != scene->entities.end(); ++it) {
			if((*it)->id != id) continue;

			scene->entities.erase(it);
			for(auto garbage_collector : scene->garbage_collectors) {
				unsigned int bit = 1 << garbage_collector.first;
				if(old_mask & bit == 0) continue;

				garbage_collector.second->collect(id);
			}
			return;
		}
	}
}
