#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <memory>

#include "../events/Logger.h"
#include "Window.h"

namespace P1::engine { class Engine; }

namespace P1::windows {
  class WindowManager {
    P1::events::Logger logger{"WindowManager"};
    std::vector<std::shared_ptr<Window>> windows{};
    bool initialized = false;

    friend engine::Engine;

    WindowManager() = default;
  
  public:
    void run();

    bool create_window(int width, int height, const char* name);
  };
}
