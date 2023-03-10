#include <algorithm>
#include "InputManager.h"

#define MAX_KEYS 1024
#define MAX_MOUSE_BUTTONS 32

namespace P1 { namespace graphics { class Window; class WindowManager; }}

namespace P1 { namespace inputs {
	InputListener::InputListener(graphics::Window* window) {
		for(int i = 0; i < sizeof(keys) / sizeof(bool); i++) keys[i] = false;
		for(int i = 0; i < sizeof(mouseButtons) / sizeof(bool); i++) mouseButtons[i] = false;
		context = window->glfwWindow;
		event_manager = std::make_unique<events::EventManager<void>>();
		InputManager::listeners.push_back(this);
	}

	InputListener::~InputListener() {
		InputManager::listeners.erase(std::find(InputManager::listeners.begin(), InputManager::listeners.end(), this));
	}

	void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		for(std::vector<InputListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			if((*it)->context == window) {
				if(action == GLFW_PRESS)
					(*it)->keys[key] = true;
				else if(action == GLFW_RELEASE)
					(*it)->keys[key] = false;
				(*it)->event_manager->emit();
				return;
			}
		}
	}
	void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		for(std::vector<InputListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			if((*it)->context == window) {
				if(action == GLFW_PRESS)
					(*it)->mouseButtons[button] = true;
				else if(action == GLFW_RELEASE)
					(*it)->mouseButtons[button] = false;
				(*it)->event_manager->emit();
				return;
			}
		}
	}
	void InputManager::cursorPosCallback(GLFWwindow* window, double x, double y) {
		for(std::vector<InputListener*>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			if((*it)->context == window) {
				(*it)->mousePos.x = x;
				(*it)->mousePos.y = y;
				(*it)->event_manager->emit();
				return;
			}
		}
	}
}}
