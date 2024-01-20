#pragma once

#include "../windows/WindowManager.h"

namespace P1::engine {
  class Engine {
  private:
    bool is_running = false;

  public:
    windows::WindowManager window_manager{};

    void run();

    inline const bool get_running_status() { return this->is_running; }
  };
}
