#include "Window.h"
#include <functional>
#include "../systems/MainManager.h"

namespace P1 { namespace graphics {
/*
	void WindowManager::start() {
		while(windows.size() > 0) {
			std::list<Window*>::iterator badwindow = windows.end();
			for(std::list<Window*>::iterator w = windows.begin(); w != windows.end(); ++w) {
				if(glfwWindowShouldClose((*w)->glfwWindow)) {
					(*w)->state = INVALID_WINDOW;
					glfwDestroyWindow((*w)->glfwWindow);
					badwindow = w;
				}
				else if((*w)->state == NEW_WINDOW) {
					glfwSetKeyCallback((*w)->glfwWindow, P1::inputs::InputManager::keyCallback);
					glfwSetMouseButtonCallback((*w)->glfwWindow, P1::inputs::InputManager::mouseButtonCallback);
					glfwSetCursorPosCallback((*w)->glfwWindow, P1::inputs::InputManager::cursorPosCallback);
					(*w)->event_manager->emit(WINDOW_START_EVENT);
					(*w)->state = VALID_WINDOW;
				}
				else {
					int width, height;
					glfwMakeContextCurrent((*w)->glfwWindow);
					glfwGetWindowSize((*w)->glfwWindow, &width, &height);
					glViewport(0, 0, width, height);
					(*w)->update();
				}
			}
			if(badwindow != windows.end()) {
				windows.erase(badwindow);
				badwindow = windows.end();
			}
		}

		glfwTerminate();
	}
*/
	Window::Window(use_create_method assertion, const char* name, int width, int height) {
		this->name = name;
		this->width = width;
		this->height = height;

		if(!init()) {
			glfwTerminate();
			state = INVALID_WINDOW;
		}
		glfwMakeContextCurrent(NULL);

		event_manager = std::make_unique<events::EventManager<int>>();
		input_listener = std::make_shared<inputs::InputListener>(this);
	}

	Window* Window::create(const char* name, int width, int height) {
		std::shared_ptr<Window> window = std::make_shared<Window>(use_create_method{}, name, width, height);
		systems::MainManager::windows.push_back(window);
		return window.get();
	}

	void Window::update() {
		glfwMakeContextCurrent(glfwWindow);
		event_manager->emit(WINDOW_UPDATE_EVENT);
		glfwSwapBuffers(glfwWindow);
		glfwGetFramebufferSize(glfwWindow, &this->width, &this->height);
		glfwPollEvents();
		glfwMakeContextCurrent(NULL);
	}

	bool Window::init() {
		glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);
		if(!glfwWindow) return false;

		return true;
	}

	void Window::clear() const {
		glfwMakeContextCurrent(glfwWindow);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwMakeContextCurrent(NULL);
	}
}}
