#pragma once
#include <GLFW/glfw3.h>
#include "../graphics/Window.h"
#include "../math/Vector2.h"
#include "../events/EventManager.h"

#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 32

namespace P1 { namespace graphics { class Window; class WindowManager; }}

namespace P1 { namespace inputs {
	class InputManager;
	class Axis;
	class InputListener {
	private:
		friend InputManager;
		friend Axis;
		GLFWwindow* context;
		std::unique_ptr<P1::events::VoidManager> eventManager;
		P1::math::Vector2<double> mousePos{};
		bool keys[MAX_KEYS];
		bool mouseButtons[MAX_MOUSE_BUTTONS];
	public:
		InputListener(P1::graphics::Window* window);
		~InputListener();

		inline P1::math::Vector2<double> getMousePos() { return mousePos; }
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

		friend class P1::graphics::WindowManager;
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
	};
}}
