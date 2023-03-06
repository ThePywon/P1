#pragma once

#include "../entity/EntitySelector.h"

namespace P1::systems { class MainManager; }

namespace P1::systems {
	#define SYSTEM_FRAME_BASED_MODE 0
	#define SYSTEM_WINDOW_BASED_MODE 1

	class SystemBase {
	private:
		friend MainManager;
		unsigned int update_mode = SYSTEM_FRAME_BASED_MODE;

		virtual void on_signature_change(entity::Entity& entity) { }

		virtual void update() {}
	public:
		SystemBase() = default;
		SystemBase(unsigned int mode) : update_mode(mode) { }
	};

	template <concepts::Component... T>
	class System : public SystemBase {
	private:
		unsigned int mode;

		friend MainManager;
		entity::EntitySelector<T...> selector{};
		inline static std::vector<entity::Entity*> entities{};

	public:
		System() = default;
		System(unsigned int mode) : SystemBase(mode), mode(mode) { }

	private:
		void on_signature_change(entity::Entity& entity) {
			bool match = selector.match(entity);

			std::vector<std::vector<entity::Entity*>::iterator> garbage{};

			for(auto it = entities.begin(); it != entities.end(); ++it)
				if(*it == nullptr || *it == &entity && !match)
					garbage.push_back(it);

			for(auto g : garbage)
				entities.erase(g);

			if(match)
				entities.push_back(&entity);
		}

		void update() {
			for(auto it = entities.begin(); it != entities.end(); ++it) {
				if(*it == nullptr) continue;

				run(*it);
			}
		}
	public:
		virtual void run(entity::Entity* entity) {}
	};
}
