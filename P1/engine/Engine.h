#pragma once

#include "../windows/WindowManager.h"

namespace P1::engine {
  class Engine {
  public:
    windows::WindowManager window_manager{};

    void run();
  };
}
