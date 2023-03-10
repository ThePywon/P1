#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include "../concepts/GameConcepts.h"
#include "../events/Logger.h"

namespace P1::graphics { class Window; }
namespace P1::entity {
	class Scene; class Entity;
	template <concepts::Component T, concepts::Component... U> class EntitySelector;
}

namespace P1::systems {
	class SystemBase;

	#define STATE_OK 0
	#define STATE_BAD 1

	class MainManager {
	public:
		inline const static std::unique_ptr<events::Logger> logger =
			std::make_unique<events::Logger>("MainManager");

		inline static unsigned int state = STATE_OK;
	private:
		friend void on_signature_change(entity::Entity* entity);

		friend graphics::Window;
		friend entity::Scene;
		inline static std::vector<entity::Scene*> scenes{};
		inline static std::vector<std::shared_ptr<SystemBase>> systems{};
	public:
		inline static std::vector<entity::Entity*> cameras{};

		inline static std::list<std::shared_ptr<graphics::Window>> windows{};
private:
		static void update();
	public:
		template <concepts::System T>
		static void add_system() {
			static bool exists = false;

			if(exists) return;

			systems.push_back(std::make_shared<T>());

			exists = true;
		}

		static void init();

		inline static void terminate() { state = STATE_BAD; }
	};
}
