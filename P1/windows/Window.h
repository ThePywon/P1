#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include <string>

namespace P1::windows {
  class WindowManager;

  class Window {
    GLFWwindow* glfw_window;

    std::string name;

    friend WindowManager;

    struct friends_only {
      explicit friends_only() = default;
    };

  public:
    Window(friends_only assertion, int width, int height, const char* name);

    ~Window();

    inline operator bool() { return glfw_window; }
  };
}
