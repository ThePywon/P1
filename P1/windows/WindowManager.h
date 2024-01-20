#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GLFW/glfw3.h>
#endif

#include <vector>
#include <memory>

#include "../events/Logger.h"
#include "Window.h"

namespace P1::engine { class Engine; }

namespace P1::windows {
  class WindowManager {
    std::vector<std::shared_ptr<Window>> windows{};
    bool initialized = false;

    friend engine::Engine;

    void run();

    explicit WindowManager() = default;

  public:
    bool create_window(int width, int height, const char* name);

  private:
    static void glew_setup();
  };
}
