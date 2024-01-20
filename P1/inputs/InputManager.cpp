#include "InputManager.h"

namespace P1::inputs {
  std::unique_ptr<events::EventManager<void, int, int, int, int>> InputManager::key_event_manager =
    std::make_unique<events::EventManager<void, int, int, int, int>>();
  std::unique_ptr<events::EventManager<void, int, int, int>> InputManager::mouse_event_manager =
    std::make_unique<events::EventManager<void, int, int, int>>();
  bool InputManager::keys[MAX_KEYS] = {};
  bool InputManager::mouseButtons[MAX_MOUSE_BUTTONS] = {};

  void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS) keys[key] = true;
    else if(action == GLFW_RELEASE) keys[key] = false;

    key_event_manager->emit(key, scancode, action, mods);
  }

  void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS) mouseButtons[button] = true;
    else if(action == GLFW_RELEASE) mouseButtons[button] = false;

    mouse_event_manager->emit(button, action, mods);
  }
}
