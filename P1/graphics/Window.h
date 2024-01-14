/*#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include <memory>
#include <map>
#include <vector>
#include "../math/Vector2.h"
#include "../events/EventManager.h"
#include "../inputs/InputManager.h"

namespace P1::inputs { class InputAxis; class InputListener; }
namespace P1::systems { class MainManager; }

#define NEW_WINDOW 0
#define VALID_WINDOW 1
#define INVALID_WINDOW 2

#define WINDOW_START_EVENT 0
#define WINDOW_UPDATE_EVENT 1

namespace P1::graphics {
	class Window : std::enable_shared_from_this<Window> {
	private:
		int width, height;

		friend systems::MainManager;
		friend P1::inputs::InputListener;
	public:
		GLFWwindow* glfwWindow;
	private:
		int state = NEW_WINDOW;

		struct use_create_method {
			explicit use_create_method() = default;
		};
	public:
		const char* name;

		std::unique_ptr<events::EventManager<unsigned int>> event_manager;
		std::shared_ptr<inputs::InputListener> input_listener;

		Window(use_create_method assertion, const char* name, int width, int height);

		static Window* create(const char* name, int width, int height);

		void clear() const;

		inline bool isValid() const {
			return state != INVALID_WINDOW;
		}

		inline P1::math::Vector2<int> get_size() const { return P1::math::Vector2<int>(width, height); }
	private:
		bool init();
	};
}
*/