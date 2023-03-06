#include "Window.h"
#include <functional>
#include "../systems/MainManager.h"

namespace P1::graphics {
	Window::Window(use_create_method assertion, const char* name, int width, int height) {
		this->name = name;
		this->width = width;
		this->height = height;

		if(!init()) {
			glfwTerminate();
			state = INVALID_WINDOW;
		}
		glfwMakeContextCurrent(NULL);

		event_manager = std::make_unique<events::EventManager<unsigned int>>();
		input_listener = std::make_shared<inputs::InputListener>(this);
	}

	Window* Window::create(const char* name, int width, int height) {
		std::shared_ptr<Window> window = std::make_shared<Window>(use_create_method{}, name, width, height);
		systems::MainManager::windows.push_back(window);
		return window.get();
	}

	bool Window::init() {
		glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);
		if(!glfwWindow) return false;

		return true;
	}

	void Window::clear() const {
		GLFWwindow* prev_context = glfwGetCurrentContext();
		glfwMakeContextCurrent(glfwWindow);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwMakeContextCurrent(prev_context);
	}
}
