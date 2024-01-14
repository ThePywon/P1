#include "Window.h"

namespace P1::windows {
  Window::Window(friends_only assertion, int width, int height, const char* name) {
    this->name = name;
    glfw_window = glfwCreateWindow(width, height, name, NULL, NULL);
    glfwMakeContextCurrent(glfw_window);
  }

  Window::~Window() {
    glfwDestroyWindow(glfw_window);
  }
}
