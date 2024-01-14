/*#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include "../graphics/Window.h"
#include "../math/Vector2.h"
#include "../events/EventManager.h"

#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 32

namespace P1::graphics { class Window; }
namespace P1::systems { class MainManager; }

namespace P1::inputs {
	#define UPDATE 0

	class InputManager;
	class Axis;
	class InputListener {
	private:
		friend InputManager;
		friend Axis;
		GLFWwindow* context;
		std::unique_ptr<events::EventManager<void>> event_manager;
		math::Vector2<double> mousePos{};
		bool keys[MAX_KEYS];
		bool mouseButtons[MAX_MOUSE_BUTTONS];
	public:
		InputListener(graphics::Window* window);
		~InputListener();

		inline math::Vector2<double> getMousePos() { return mousePos; }
		inline bool isKeyPressed(int key) {
			if(key >= MAX_KEYS) return false;
			return keys[key];
		}
		inline bool isMouseButtonPressed(int button) {
			if(button >= MAX_MOUSE_BUTTONS) return false;
			return mouseButtons[button];
		}
	};

	class InputManager {
	private:
		friend InputListener;
		inline static std::vector<InputListener*> listeners{};

		friend class systems::MainManager;
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
	};
}
*/