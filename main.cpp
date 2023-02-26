#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

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

Scene scene{};
/*
Entity* rObj = Entity::create(&scene, "Raw Input");
Entity* lObj = Entity::create(&scene, "Linear Input");
Entity* sObj = Entity::create(&scene, "Smooth Input");

LineRendererComponent* rLine = rObj->add_component<LineRendererComponent>();
LineRendererComponent* lLine = lObj->add_component<LineRendererComponent>();
LineRendererComponent* sLine = sObj->add_component<LineRendererComponent>();
*/

Entity* camera = Entity::create(&scene, "Camera");
Entity* test_lines = Entity::create(&scene, "Test Lines");

Transform<>* camera_transform;
Viewport* camera_viewport;
Transform<>* line_transform;
LineRendererComponent* line_rend;

void glew_setup();
void start();
void update();
int main(int argc, char *argv[]) {

	// Init glfw
	if(!glfwInit()) return 1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Init windows
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	window = Window::create("test", 600, 600);

	Arrows = std::make_unique<Axis2D>(&*window, GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_RIGHT, GLFW_KEY_DOWN);
/*
	rLine->vertices.push_back(0);
	rLine->vertices.push_back(0);
	rLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	lLine->vertices.push_back(0);
	sLine->vertices.push_back(0);
	sLine->vertices.push_back(0);
	sLine->vertices.push_back(0);

	rLine->color = SolidColor<float>(1, 0, 0);
	lLine->color = SolidColor<float>(0, 1, 0);
	sLine->color = SolidColor<float>(0, 0, 1);
*/
	// Create event listeners
	window->event_manager->on(WINDOW_START_EVENT, glew_setup);
	window->event_manager->on(WINDOW_START_EVENT, start);
	window->event_manager->on(WINDOW_UPDATE_EVENT, update);

	MainManager::add_system<LineRenderer>();

	// Start update loop
	MainManager::init();

	return 0;
}

void glew_setup() {
	int state = glewInit();
	if(state == GLEW_OK) {
		std::cout << "Glew initialized successfully." << std::endl;
		std::cout << "Currently using: " << glGetString(GL_VERSION) << std::endl;
	}
	else {
		std::cout << "ERROR::GLEW::NOT_OK\n" << glewGetErrorString(state) << std::endl;
	}
}

// Start callback
void start() {
	std::cout << "Start event called!" << std::endl;

	test_lines->materials.push_back(std::make_shared<Material>());

	camera_transform = camera->add_component<Transform<>>();
	camera_viewport = camera->add_component<Viewport>();
	line_transform = test_lines->add_component<Transform<>>();
	line_rend = test_lines->add_component<LineRendererComponent>();
	line_rend->color = SolidColor<float>(1, 1, 1);
}

void update() {
	glClearColor(0, 0, 0, 0);
	window->clear();

	line_transform->position.x = *Arrows->smooth().x;
	line_transform->position.y = *Arrows->smooth().y;
	camera_transform->position.x = 0.5;

	/*rLine->vertices[1] = Arrows->raw();
	lLine->vertices[1] = Arrows->linear();
	sLine->vertices[1] = Arrows->smooth();*/
}
