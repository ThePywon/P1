#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iterator>

#include "WindowManager.h"

namespace P1::windows {
  void glew_setup();
  void WindowManager::run() {
    while(windows.size() > 0) {
      std::vector<std::shared_ptr<Window>>::iterator bad_window = windows.end();

      std::vector<std::shared_ptr<Window>>::iterator window;
      for(window = windows.begin(); window != windows.end(); window++) {
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

        glfwSwapBuffers(w_ptr);
        glfwPollEvents();
      }

      // Let the window close and stop handling it
      if(bad_window != windows.end()) {
        logger.debug("Window \"" + (*bad_window)->name + "\" closed!");
        windows.erase(bad_window);
      }
    }

    glfwTerminate();
  }

  bool WindowManager::create_window(int width, int height, const char* name) {
    // Create the window
    std::shared_ptr<Window> window = std::make_shared<Window>(Window::friends_only{}, width, height, name);

    // Make sure it initialized properly
    if(!window) return false;

    windows.push_back(window);

    if(!initialized) {
      glew_setup();
      initialized = true;
    }

    return true;
  }

  // Initialises opengl functions
  // Only needs to be run once
  // MUST HAVE A VALID WINDOW CONTEXT BEFORE RUNNING
  void glew_setup() {
    P1::events::Logger glew_logger{"GLEW"};

    int state = glewInit();
    if(state == GLEW_OK) {
      glew_logger.debug("Initialisation successfull.");
      glew_logger.debug("Currently using: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    }
    else {
      glew_logger.crit("Initialisation FAILED.");
      glew_logger.crit("Details: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(state))));
    }
  }
}
