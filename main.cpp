#include <GLFW/glfw3.h>
#include <memory>
#include <thread>
#include "P1/math/Vector2.h"
#include "P1/math/Vector3.h"
#include "P1/graphics/Window.h"
#include "P1/inputs/Axis.h"
#include "P1/inputs/InputManager.h"
#include "P1/graphics/Line.h"
#include "P1/math/Matrix.h"
#include "P1/miscellaneous/Proxy.h"

using namespace P1;
using namespace inputs;
using namespace graphics;
using namespace events;
using namespace math;

std::unique_ptr<Window> window;
std::unique_ptr<Window> window2;
std::unique_ptr<Window> testwindow;

std::unique_ptr<Axis> Raxis;
std::unique_ptr<Axis> Gaxis;
std::unique_ptr<Axis> Baxis;
std::unique_ptr<Axis> Taxis;
std::unique_ptr<Axis2D> Arrows;

std::unique_ptr<Line<double>> rLine;
std::unique_ptr<Line<double>> lLine;
std::unique_ptr<Line<double>> sLine;

void start();
void update();
void update2();
int main() {
	// Init windows
	window = std::make_unique<Window>("test", 600, 600);
	window2 = std::make_unique<Window>("test", 600, 600);

	Raxis = std::make_unique<Axis>(&*window, std::vector<int>{GLFW_KEY_Q}, std::vector<int>{GLFW_KEY_A});
	Gaxis = std::make_unique<Axis>(&*window, std::vector<int>{GLFW_KEY_W}, std::vector<int>{GLFW_KEY_S});
	Baxis = std::make_unique<Axis>(&*window, std::vector<int>{GLFW_KEY_E}, std::vector<int>{GLFW_KEY_D});
	Taxis = std::make_unique<Axis>(&*window, std::vector<int>{GLFW_KEY_R}, std::vector<int>{});
	Arrows = std::make_unique<Axis2D>(&*window, std::vector<int>{GLFW_KEY_LEFT},
			std::vector<int>{GLFW_KEY_UP}, std::vector<int>{GLFW_KEY_RIGHT}, std::vector<int>{GLFW_KEY_DOWN});

	// Create event listeners
	window->eventManager->on("start", start);
	window->eventManager->on("update", update);
	window2->eventManager->on("start", start);
	window2->eventManager->on("update", update2);

	// Start update loop
	P1::graphics::WindowManager::start();

	return 0;
}

// Start callback
void start() {
	std::cout << "Start!" << std::endl;
	rLine = std::make_unique<Line<double>>(Vector2<double>::zero(), Vector2<double>::one() * 0.5f, SolidColor<float>(1, 0, 0));
	lLine = std::make_unique<Line<double>>(Vector2<double>::zero(), Vector2<double>::one() * 0.5f, SolidColor<float>(0, 1, 0));
	sLine = std::make_unique<Line<double>>(Vector2<double>::zero(), Vector2<double>::one() * 0.5f, SolidColor<float>(0, 0, 1));
}

void teststart() {
	std::cout << "Test Start!" << std::endl;
}

void testupdate();
void update() {
	if(Taxis->raw() > 0 && (!testwindow || !testwindow->isValid())) {
		testwindow = std::make_unique<Window>("test2", 600, 600);
		testwindow->eventManager->on("start", teststart);
		testwindow->eventManager->on("update", testupdate);
	}

	glClearColor((Raxis->smooth() + 1) * 0.5f, (Gaxis->smooth() + 1) * 0.5f, (Baxis->smooth() + 1) * 0.5f, 1);
	window->clear();

	rLine->points[1] = Arrows->raw();
	lLine->points[1] = Arrows->linear();
	sLine->points[1] = Arrows->smooth();

	rLine->render();
	lLine->render();
	sLine->render();
}

void update2() {
	glClearColor(0.71764705882352941176, 0.43137254901960784314, 0.47450980392156862745, 1);
	window2->clear();
}

float r = 0;
float g = 1;
float b = 0;

bool t = false;

void testupdate() {
	if(!t) {
		if(r < 1)
			r += 0.1f;
		else if(g > 0)
			g -= 0.1f;
		else if(b < 1)
			b += 0.1f;
		else t = true;
	}
	else {
		if(r > 0)
			r -= 0.1f;
		else if(g < 1)
			g += 0.1f;
		else if(b > 0)
			b -= 0.1f;
		else t = false;
	}

	glClearColor(r, g, b, 1);
	testwindow->clear();
}
