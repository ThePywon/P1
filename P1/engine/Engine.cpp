#include "Engine.h"

namespace P1::engine {
  void Engine::run() {
    this->is_running = true;
    this->window_manager.run();
    this->is_running = false;
  }
}
