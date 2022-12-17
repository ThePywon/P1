#include <GLFW/glfw3.h>
#include <functional>
#include <memory>
#include "Window.h"
#include "../inputs/InputManager.h"
#include "../events/EventManager.h"

namespace P1 { namespace graphics {

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
					(*w)->eventManager->emit("start");
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

	Window::Window(const char* name, int width, int height) {
		this->name = name;
		this->width = width;
		this->height = height;

		if(!init()) glfwTerminate();
		glfwMakeContextCurrent(NULL);

		eventManager = std::make_unique<P1::events::EventManager<std::string>>();
		inputListener = std::make_shared<P1::inputs::InputListener>(this);

		WindowManager::windows.push_back(this);
	}

	void Window::update() {
		glfwMakeContextCurrent(glfwWindow);
		eventManager->emit("update");
		glfwSwapBuffers(glfwWindow);
		glfwGetFramebufferSize(glfwWindow, &this->width, &this->height);
		glfwPollEvents();
	}

	bool Window::init() {
		if(!glfwInit()) return false;

		glfwWindow = glfwCreateWindow(width, height, name, NULL, NULL);
		if(!glfwWindow) return false;

		return true;
	}

	void Window::clear() const {
		glfwMakeContextCurrent(glfwWindow);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}}
