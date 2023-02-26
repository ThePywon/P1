#pragma once

#include "../entity/EntitySelector.h"

namespace P1::systems { class MainManager; }

namespace P1::systems {
	#define SYSTEM_FRAME_BASED_MODE 0
	#define SYSTEM_WINDOW_BASED_MODE 1

	class SystemBase {
	private:
		friend MainManager;
		unsigned int update_mode;

		virtual void on_signature_change(entity::Entity* entity) {}

		virtual void update() {}
	public:
		SystemBase(unsigned int mode = SYSTEM_FRAME_BASED_MODE) : update_mode(mode) { }
	};

	template <concepts::Component... T>
	class System : public SystemBase {
	private:
		friend MainManager;
		entity::EntitySelector<T...> selector{};
		inline static std::vector<entity::Entity*> entities{};

	public:
		System(unsigned int mode = SYSTEM_FRAME_BASED_MODE) : SystemBase(mode) { }

	private:
		void on_signature_change(entity::Entity* entity) {
			bool match = selector.match(entity);

			for(auto it = entities.begin(); it != entities.end(); ++it) {
				if(*it == nullptr) continue;

				if(*it == entity) {
					if(!match) entities.erase(it);
					return;
				}
			}

			if(match)
				entities.push_back(entity);
		}

		void update() {
			std::vector<std::vector<entity::Entity*>::iterator> garbage{};

			for(auto it = entities.begin(); it != entities.end(); ++it) {
				if(*it == nullptr) {
					garbage.push_back(it);
					continue;
				}

				run(*it);
			}

			for(auto g : garbage)
				entities.erase(g);
		}
	public:
		virtual void run(entity::Entity* entity) {}
	};
}
