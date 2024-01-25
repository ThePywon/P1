#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <memory>
#include <iostream>
#include <thread>

#include "P1/miscellaneous/Proxy.h"
#include "P1/math/Vector2.h"
#include "P1/engine/Engine.h"
#include "P1/events/EventManager.h"
#include "P1/inputs/InputManager.h"
#include "P1/inputs/Axis.h"
#include "P1/inputs/Axis2.h"

using namespace P1;
using namespace math;

void glfw_error_callback(int error, const char* description) {
  std::cerr << "Error: " << description << '\n' << std::endl;
}

int main(int argc, char *argv[]) {
	// Init glfw
  glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) return 1;

  // Version hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  engine::Engine p1{};

  p1.window_manager.create_window(300, 300, "Engine");
  p1.window_manager.create_window(300, 300, "Engine 2");
  p1.window_manager.create_window(300, 300, "Engine 3");

  inputs::Axis x{GLFW_KEY_RIGHT, GLFW_KEY_LEFT};
  inputs::Axis y{GLFW_KEY_UP, GLFW_KEY_DOWN};
  inputs::Axis2 arrows{x, y};

  std::thread{[&p1,&arrows](){
    while(p1.get_running_status()) {
      std::cout << arrows.get_raw() << std::endl;
    }
  }}.detach();

  p1.run();

	return 0;
}
