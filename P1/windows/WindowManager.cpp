#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
#include <iterator>

#include "WindowManager.h"
#include "../inputs/InputManager.h"
#include "../inputs/Axis2.h"

namespace P1::windows {
  void WindowManager::run() {
    while(windows.size() > 0) {
      auto bad_window = windows.end();

      for(auto window = windows.begin(); window != windows.end(); window++) {
        GLFWwindow* w_ptr = (*window)->glfw_window;

        // Flag window as invalid if it should close
        if(glfwWindowShouldClose(w_ptr)) {
          bad_window = window;
          continue;
        }

        // Rendering shit
        glfwMakeContextCurrent(w_ptr);

        int width, height;
        glfwGetFramebufferSize(w_ptr, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(1, 0.5, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        inputs::Axis x{GLFW_KEY_RIGHT, GLFW_KEY_LEFT};
        inputs::Axis y{GLFW_KEY_UP, GLFW_KEY_DOWN};
        inputs::Axis2 arrows{x, y};
        math::vector2 raw = arrows.get_raw();

        glPointSize(10);
	      glLineWidth(2.5); 
        glColor3f(0, 1, 0);
        glBegin(GL_POINTS);
        glVertex3f(0, 0, 0);
        glVertex3f(raw.x, raw.y, 0);
        glEnd();
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(raw.x, raw.y, 0);
        glEnd();

        glfwSwapBuffers(w_ptr);
        glfwPollEvents();
      }

      // Let the window close and stop handling it
      if(bad_window != windows.end())
        windows.erase(bad_window);
    }

    glfwTerminate();
  }

  bool WindowManager::create_window(int width, int height, const char* name) {
    // Create the window
    std::shared_ptr<Window> window = std::make_shared<Window>(Window::friends_only{}, width, height, name);

    // Make sure it initialized properly
    if(!*window) return false;

    windows.push_back(window);

    if(!initialized) {
      WindowManager::glew_setup();
      initialized = true;
    }

    inputs::InputManager::registerWindow(window->glfw_window);

    return true;
  }

  // Initialises opengl functions
  // Only needs to be run once
  // MUST HAVE A VALID WINDOW CONTEXT BEFORE RUNNING
  void WindowManager::glew_setup() {
    P1::events::Logger glew_logger{"GLEW"};

    int state = glewInit();
    if(state == GLEW_OK) {
      glew_logger.debug_sync("Initialisation successfull.");
      glew_logger.debug_sync("Currently using: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    }
    else {
      glew_logger.crit_sync("Initialisation FAILED.");
      glew_logger.crit_sync("Details: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(state))));
    }
  }
}
