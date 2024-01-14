#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iterator>

#include "WindowManager.h"

namespace P1::windows {
  bool glew_initialized = false;

  void glew_setup();
  void WindowManager::run() {
    while(windows.size() > 0) {
      std::vector<GLFWwindow*>::iterator bad_window = windows.end();

      std::vector<GLFWwindow*>::iterator window;
      for(window = windows.begin(); window != windows.end(); window++) {

        // Flag window as invalid if it should close
        if(glfwWindowShouldClose(*window)) {
          bad_window = window;
          continue;
        }

        // Rendering shit
        glfwMakeContextCurrent(*window);

        int width, height;
        glfwGetFramebufferSize(*window, &width, &height);
        glViewport(0, 0, width, height);

        glClearColor(1, 0.5, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(*window);
        glfwPollEvents();
      }

      // Let the window close and stop handling it
      if(bad_window != windows.end()) {
        logger.debug("Window closed!");
        glfwDestroyWindow(*bad_window);
        windows.erase(bad_window);
      }
    }

    glfwTerminate();
  }

  bool WindowManager::create_window(int width, int height, const char* name) {
    GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
    windows.push_back(window);

    if(!glew_initialized) {
      glfwMakeContextCurrent(window);
      glew_setup();
      glew_initialized = true;
    }

    return window;
  }

  // Initialises opengl functions
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
