#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <list>
#include "../math/Vector2.h"
#include "../events/EventManager.h"
#include "../inputs/InputManager.h"

namespace P1 { namespace inputs { class InputAxis; class InputListener; }}

namespace P1 { namespace graphics {

	#define NEW_WINDOW 0
	#define VALID_WINDOW 1
	#define INVALID_WINDOW 2

	class Window;
	class WindowManager {
	private:
		friend Window;
		inline static std::list<Window*> windows{};
	public:
		static void start();
	};

	class Window {
	private:
		int width, height;

		friend WindowManager;
		friend P1::inputs::InputListener;
		GLFWwindow* glfwWindow;
		int state = NEW_WINDOW;
	public:
		const char* name;

		std::unique_ptr<P1::events::EventManager<std::string>> eventManager;
		std::shared_ptr<P1::inputs::InputListener> inputListener;

		Window(const char* name, int width, int height);

		void clear() const;

		void update();

		inline bool isValid() const {
			return state != INVALID_WINDOW;
		}

		inline P1::math::Vector2<int> getSize() const { return P1::math::Vector2<int>(width, height); }
	private:
		bool init();

		//Window& operator = (Window&& other) = delete;
	};
}}
