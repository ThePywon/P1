#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include <memory>

#include "../events/EventManager.h"

namespace P1::windows { class WindowManager; }

namespace P1::inputs {
  #define MAX_KEYS 1024
  #define MAX_MOUSE_BUTTONS 32

  class InputManager {
  private:
    static std::unique_ptr<events::EventManager<void, int, int, int, int>> key_event_manager;
    static std::unique_ptr<events::EventManager<void, int, int, int>> mouse_event_manager;
    static bool keys[MAX_KEYS];
    static bool mouseButtons[MAX_MOUSE_BUTTONS];

    friend windows::WindowManager;

    inline static void registerWindow(GLFWwindow* window) {
      glfwSetKeyCallback(window, InputManager::keyCallback);
      glfwSetMouseButtonCallback(window, InputManager::mouseButtonCallback);
    }

  public:
		inline static bool isKeyPressed(int key) {
			if(key >= MAX_KEYS) return false;
			return keys[key];
		}
		inline static bool isMouseButtonPressed(int button) {
			if(button >= MAX_MOUSE_BUTTONS) return false;
			return mouseButtons[button];
		}

  private:
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
  };
}