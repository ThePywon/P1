#pragma once

#include "../concepts/GameConcepts.h"
#include "../components/Component.h"
#include "../entity/Entity.h"

namespace P1::entity {
	template <concepts::Component T, concepts::Component... U>
	struct EntitySelector {
		bool match(Entity* E) {
			return E->has_component<T>() && EntitySelector<U...>{}.match(E);
		}
	};

	template <concepts::Component T>
	struct EntitySelector<T> {
		bool match(Entity* E) {
			return E->has_component<T>();
		}
	};
}
