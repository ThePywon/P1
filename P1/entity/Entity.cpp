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
		return entity.get();
	}
}
