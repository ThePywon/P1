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
#include "P1/events/Logger.h"

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

Entity* camera = Entity::create(&scene, "Camera");
Entity* test_lines = Entity::create(&scene, "Test Lines");
Entity* world_center = Entity::create(&scene, "World Center");

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

	// Create event listeners
	window->event_manager->on(WINDOW_START_EVENT, glew_setup);
	window->event_manager->on(WINDOW_START_EVENT, start);
	window->event_manager->on(WINDOW_UPDATE_EVENT, update);

	MainManager::add_system<LineRenderer>();

	// Start update loop
	MainManager::init();

	return MainManager::state;
}

void glew_setup() {
	Logger glew_logger{"Glew"};

	int state = glewInit();
	if(state == GLEW_OK) {
		glew_logger.debug("initialized successfully");
		glew_logger.debug("Currently using: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
	}
	else {
		glew_logger.debug("initialized unsuccessfully.\n" + std::string(reinterpret_cast<const char*>(glewGetErrorString(state))));
	}
}

// Start callback
void start() {
	world_center->materials.push_back(std::make_shared<Material>());
	world_center->add_component<Transform<>>();
	LineRendererComponent* center_rend = world_center->add_component<LineRendererComponent>();
	center_rend->color = SolidColor<float>(0, 0, 1);

	test_lines->materials.push_back(std::make_shared<Material>());

	camera_transform = camera->add_component<Transform<>>();
	camera_viewport = camera->add_component<Viewport>();
	line_transform = test_lines->add_component<Transform<>>();
	line_rend = test_lines->add_component<LineRendererComponent>();
	line_rend->color = SolidColor<float>(1, 1, 1);
	camera_transform->scale = Vector3(2.0f, 2.0f, 2.0f);
}

Logger test_logger{"FUCK YOU"};

void update() {
	glClearColor(0, 0, 0, 0);
	window->clear();

	line_transform->position.x = *Arrows->smooth().x;
	line_transform->position.y = *Arrows->smooth().y;

	test_logger.error("Bro this shit gotta go.");
}
