#include <GLFW/glfw3.h>
#include <memory>
#include "P1/math/Vector2.h"
#include "P1/math/Vector3.h"
#include "P1/graphics/Window.h"
#include "P1/inputs/Axis.h"
#include "P1/inputs/InputManager.h"
#include "P1/graphics/LineRenderer.h"
#include "P1/math/Matrix.h"
#include "P1/miscellaneous/Proxy.h"
#include "P1/entity/Transform.h"
#include "P1/graphics/Camera.h"
#include "P1/entity/Entity.h"

using namespace P1;
using namespace inputs;
using namespace graphics;
using namespace events;
using namespace math;
using namespace entity;

std::unique_ptr<Window> window;

std::unique_ptr<Axis2D> Arrows;

Scene scene{};

Entity* rObj = Entity::create(&scene, "Raw Input");
Entity* lObj = Entity::create(&scene, "Linear Input");
Entity* sObj = Entity::create(&scene, "Smooth Input");

LineRenderer* rLine = Renderer::create<LineRenderer>(rObj, Vector2<>::zero(), Vector2<>::one() * 0.5f, SolidColor<float>(1, 0, 0));
LineRenderer* lLine = Renderer::create<LineRenderer>(lObj, Vector2<>::zero(), Vector2<>::one() * 0.5f, SolidColor<float>(0, 1, 0));
LineRenderer* sLine = Renderer::create<LineRenderer>(sObj, Vector2<>::zero(), Vector2<>::one() * 0.5f, SolidColor<float>(0, 0, 1));

void start();
void update();
int main() {
	Matrix<double, 4> test = Matrix<double, 4>::translate(1, 2, 3);
	Matrix<double, 4> test2 = Matrix<double, 4>::scale(1, 2, 3);
	std::cout << test << std::endl << std::endl;
	std::cout << test2 << std::endl << std::endl;
	std::cout << test * test2 << std::endl << std::endl;

	// Init glfw
	if(!glfwInit()) return 1;

	// Init windows
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	window = std::make_unique<Window>("test", 600, 600);

	Arrows = std::make_unique<Axis2D>(&*window, std::vector<int>{GLFW_KEY_LEFT},
			std::vector<int>{GLFW_KEY_UP}, std::vector<int>{GLFW_KEY_RIGHT}, std::vector<int>{GLFW_KEY_DOWN});

	// Create event listeners
	window->eventManager->on("start", start);
	window->eventManager->on("update", update);

	// Start update loop
	P1::graphics::WindowManager::start();

	return 0;
}

// Start callback
void start() {
	std::cout << "Start!" << std::endl;
}

void update() {
	glClearColor(0, 0, 0, 0);
	window->clear();

	rLine->points[1] = Arrows->raw();
	lLine->points[1] = Arrows->linear();
	sLine->points[1] = Arrows->smooth();

	rLine->render();
	lLine->render();
	sLine->render();
}
