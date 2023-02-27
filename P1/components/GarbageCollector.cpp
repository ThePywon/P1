#include "GarbageCollector.h"
#include "../entity/Entity.h"

namespace P1::components {
	unsigned int get_entity_id(entity::Entity* entity) { return entity->get_id(); }
}
