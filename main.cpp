#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "P1/math/Vector2.h"
#include "P1/math/Vector3.h"
#include "P1/graphics/Window.h"
#include "P1/systems/MainManager.h"
#include "P1/inputs/Axis.h"
#include "P1/inputs/InputManager.h"
#include "P1/math/Matrix.h"
#include "P1/miscellaneous/Proxy.h"
#include "P1/components/Transform.h"
#include "P1/components/Viewport.h"
#include "P1/entity/Entity.h"
#include "P1/components/LineRendererComponent.h"
#include "P1/systems/System.h"
#include "P1/entity/EntitySelector.h"
#include "P1/systems/LineRenderer.h"

using namespace P1;
using namespace inputs;
using namespace graphics;
using namespace events;
using namespace math;
using namespace entity;
using namespace components;
using namespace systems;

Window* window;

std::unique_ptr<Axis2D> Arrows;

Matrix<int, 4, 2> A{1, 2, 3, 4, 5, 6, 7, 8};
Matrix<int, 1, 4> B{7, -3, 0, -1};
Matrix<int, 3, 2> C{3, 0, 4, 2, -1, -8};
Matrix<int, 3> D = Matrix<int, 3>::identity();
Matrix<int, 3> E{7, 8, -4, -4, 1, -4, 2, 9, 3};

Scene scene{};
/*
Entity* rObj = Entity::create(&scene, "Raw Input");
Entity* lObj = Entity::create(&scene, "Linear Input");
Entity* sObj = Entity::create(&scene, "Smooth Input");

LineRendererComponent* rLine = rObj->add_component<LineRendererComponent>();
LineRendererComponent* lLine = lObj->add_component<LineRendererComponent>();
LineRendererComponent* sLine = sObj->add_component<LineRendererComponent>();
*/
void start();
void update();
int main(int argc, char *argv[]) {

	std::cout << (D + E) << std::endl << std::endl;
	//std::cout << (A + B) << std::endl << std::endl; INVALID
	std::cout << (D - E) << std::endl << std::endl;
	std::cout << (-2 * B) << std::endl << std::endl;
	std::cout << (A * B) << std::endl << std::endl;
	//std::cout << (B * A) << std::endl << std::endl; INVALID
	std::cout << A.transpose() << std::endl << std::endl;
	std::cout << B.transpose() << std::endl << std::endl;
	std::cout << D.transpose() << std::endl << std::endl;
	std::cout << (C * D) << std::endl << std::endl;
	std::cout << (C.transpose() * C) << std::endl << std::endl;
	std::cout << (C * C.transpose()) << std::endl << std::endl;

/*
	// Init glfw
	if(!glfwInit()) return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init windows
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	window = Window::create("test", 600, 600);

	Arrows = std::make_unique<Axis2D>(&*window, GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_RIGHT, GLFW_KEY_DOWN);

	/*rLine->vertices.push_back(0);
	rLine->vertices.push_back(0);
	rLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	sLine->vertices.push_back(0);
	sLine->vertices.push_back(0);
	sLine->vertices.push_back(0);*/
/*
	rLine->color = SolidColor<float>(1, 0, 0);
	lLine->color = SolidColor<float>(0, 1, 0);
	sLine->color = SolidColor<float>(0, 0, 1);

	// Create event listeners
	window->event_manager->on(WINDOW_START_EVENT, start);
	window->event_manager->on(WINDOW_UPDATE_EVENT, update);

	MainManager::add_system<LineRenderer>();

	// Start update loop
	MainManager::init();*/

	return 0;
}

// Start callback
void start() {
	std::cout << "Start!" << std::endl;
	glewInit();
}

void update() {
	glewInit();
	glClearColor(0, 0, 0, 0);
	/*rLine->vertices[1] = Arrows->raw();
	lLine->vertices[1] = Arrows->linear();
	sLine->vertices[1] = Arrows->smooth();*/
}
