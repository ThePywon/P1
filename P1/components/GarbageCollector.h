#pragma once

#include "../concepts/GameConcepts"
#include "Component.h"

namespace P1::components {
	class GarbageCollectorBase {
	protected:
		virtual unsigned int component_id;
	};

	template <concepts::Component T>
	class GarbageCollector : public GarbageCollectorBase {
	private:
		unsigned int component_id;
	};
}
