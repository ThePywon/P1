#include "MainManager.h"
#include "../graphics/Window.h"
#include "../inputs/InputManager.h"
#include "../entity/Entity.h"
#include "../systems/System.h"
#include "../components/Transform.h"
#include "../components/Viewport.h"

namespace P1::systems {
	void on_signature_change(entity::Entity* entity) {
		static entity::EntitySelector<components::Transform<>, components::Viewport> camera_selector{};

		bool match = camera_selector.match(entity);

		std::vector<std::vector<entity::Entity*>::iterator> garbage{};

		for(auto it = MainManager::cameras.begin(); it != MainManager::cameras.end(); ++it) {
			if(*it == nullptr) {
				garbage.push_back(it);
				continue;
			}

			if(*it == entity) {
				if(!match) MainManager::cameras.erase(it);
				continue;
			}
		}

		for(auto it : garbage)
			MainManager::cameras.erase(it);

		if(match)
			MainManager::cameras.push_back(entity);
	}

	void MainManager::init() {
		// System setup
		for(auto scene : scenes) {
			for(auto system : systems) {
				// Hook up systems with scene event managers
				scene->event_manager.funnel([system](entity::Entity* entity) {
					system->on_signature_change(entity);
					on_signature_change(entity);
				});

				// Initializing each system
				for(auto entity : scene->entities)
					system->on_signature_change(entity.get());
			}

			for(auto entity : scene->entities)
				on_signature_change(entity.get());
		}

		// Start update loop
		MainManager::update();
	}

	void MainManager::update() {
		while(windows.size() > 0) {
			auto bad_window = windows.end();
			for(auto w = windows.begin(); w != windows.end(); ++w) {
				auto window = *w;
				auto glfwWindow = window->glfwWindow;
				if(glfwWindowShouldClose(glfwWindow)) {
					window->state = INVALID_WINDOW;
					glfwDestroyWindow(glfwWindow);
					bad_window = w;
					continue;
				}
				else if(window->state == NEW_WINDOW) {
					glfwSetKeyCallback(glfwWindow, P1::inputs::InputManager::keyCallback);
					glfwSetMouseButtonCallback(glfwWindow, P1::inputs::InputManager::mouseButtonCallback);
					glfwSetCursorPosCallback(glfwWindow, P1::inputs::InputManager::cursorPosCallback);

					glfwMakeContextCurrent(glfwWindow);
					window->event_manager->emit(WINDOW_START_EVENT);
					glfwMakeContextCurrent(NULL);

					window->state = VALID_WINDOW;
					continue;
				}

				int width, height;
				glfwMakeContextCurrent(glfwWindow);
				glfwGetFramebufferSize(glfwWindow, &width, &height);
				glViewport(0, 0, width, height);
				window->width = width;
				window->height = height;

				glfwPollEvents();

				window->event_manager->emit(WINDOW_UPDATE_EVENT);
				glfwMakeContextCurrent(NULL);
			}
			if(bad_window != windows.end()) {
				windows.erase(bad_window);
				std::cout << "Window \"" << (*bad_window)->name << "\" destroyed." << std::endl;
			}

			for(auto system : systems) {
				if(system->update_mode == SYSTEM_FRAME_BASED_MODE) {
					system->update();
				}
				else if(system->update_mode == SYSTEM_WINDOW_BASED_MODE) {
					for(auto w = windows.begin(); w != windows.end(); ++w) {
						glfwMakeContextCurrent((*w)->glfwWindow);
						system->update();
					}
				}
				glfwMakeContextCurrent(NULL);
			}

			for(auto w = windows.begin(); w != windows.end(); ++w)
				glfwSwapBuffers((*w)->glfwWindow);
		}

		glfwTerminate();
	}
}
