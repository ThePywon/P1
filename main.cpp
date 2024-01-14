#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <memory>
#include <iostream>
#include "P1/miscellaneous/Proxy.h"
#include "P1/math/Vector2.h"
#include "P1/engine/Engine.h"

using namespace P1;
using namespace math;

void glfw_error_callback(int, const char*);
void glew_setup();
int main(int argc, char *argv[]) {
	// Init glfw
  glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()) return 1;

  // Version hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  engine::Engine p1{};

  p1.window_manager.create_window(300, 300, "Engine");
  p1.window_manager.create_window(300, 300, "Engine");
  p1.window_manager.create_window(300, 300, "Engine");

  p1.run();

	return 0;
}

void glfw_error_callback(int error, const char* description) {
  std::cerr << "Error: " << description << '\n' << std::endl;
}
