#pragma once

#include "MainManager.h"
#include "System.h"
#include "../components/Transform.h"
#include "../components/RendererComponent.h"
#include "../math/Matrix.h"
#include <vector>

namespace P1::systems {
	class Renderer : public System<components::Transform<>, components::RendererComponent> {
	public:
		Renderer() : System<components::Transform<>, components::RendererComponent>(SYSTEM_WINDOW_BASED_MODE) {}

		void run(entity::Entity* entity) {
			components::Transform<>* transform = entity->get_component<components::Transform<>>();
			math::Matrix<float, 4> t_mat = transform->scale.to_scale_matrix() *
				transform->position.to_translate_matrix<COLUMN_MAJOR>();

			components::RendererComponent* renderer = entity->get_component<components::RendererComponent>();

			if(entity->materials.size() == 0) {
				std::cout << "[LineRenderer] At entity \"" << entity->name <<
					"\" #" << entity->id << ": Cannot render lines without a material!" << std::endl;
				return;
			}

			entity->materials[0]->set_mesh(renderer->vertices.data(), renderer->vertices.size());

			glUseProgram(entity->materials[0]->program);
			int color = glGetUniformLocation(entity->materials[0]->program, "color");
			int mvp = glGetUniformLocation(entity->materials[0]->program, "MVP");
			int scale = glGetUniformLocation(entity->materials[0]->program, "scale");
			glUniform3fv(color, 1, renderer->color.data_ptr());
			glLineWidth(renderer->line_width);
			glPointSize(renderer->point_size);
			glBindVertexArray(entity->materials[0]->VAO);

			for(int i = 0; i < MainManager::cameras.size(); i++) {
				components::Transform<>* camera_transform = MainManager::cameras[i]->get_component<components::Transform<>>();
				components::Viewport* camera_viewport = MainManager::cameras[i]->get_component<components::Viewport>();

				math::Matrix<float, 4> MVP = t_mat * (1.0f / camera_transform->scale).to_scale_matrix() *
				(-camera_transform->position).to_translate_matrix<COLUMN_MAJOR>();

				glUniformMatrix4fv(mvp, 1, GL_FALSE, &MVP.data[0][0]);
				glDrawArrays(renderer->draw_mode, 0, renderer->vertices.size() / 3);
			}
		}
	};
}
