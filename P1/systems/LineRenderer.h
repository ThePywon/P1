#pragma once

#include "MainManager.h"
#include "System.h"
#include "../components/Transform.h"
#include "../components/LineRendererComponent.h"
#include "../math/Matrix.h"
#include <vector>

namespace P1::systems {
	class LineRenderer : public System<components::Transform<>, components::LineRendererComponent> {
	public:
		LineRenderer() : System<components::Transform<>, components::LineRendererComponent>(SYSTEM_WINDOW_BASED_MODE) {}

		void run(entity::Entity* entity) {
			components::Transform<>* transform = entity->get_component<components::Transform<>>();
			math::Matrix<float, 4> t_mat = transform->scale.to_scale_matrix() *
				transform->position.to_translate_matrix<COLUMN_MAJOR>();

			components::LineRendererComponent* renderer = entity->get_component<components::LineRendererComponent>();

			entity->materials[0]->set_mesh(renderer->vertices.data(), renderer->vertices.size());

			glUseProgram(entity->materials[0]->program);
			int color = glGetUniformLocation(entity->materials[0]->program, "color");
			int mvp = glGetUniformLocation(entity->materials[0]->program, "MVP");
			int scale = glGetUniformLocation(entity->materials[0]->program, "scale");
			glUniform3fv(color, 1, renderer->color.data_ptr());
			glLineWidth(renderer->line_width);
			glBindVertexArray(entity->materials[0]->VAO);

			for(int i = 0; i < MainManager::cameras.size(); i++) {
				components::Transform<>* camera_transform = MainManager::cameras[i]->get_component<components::Transform<>>();
				components::Viewport* camera_viewport = MainManager::cameras[i]->get_component<components::Viewport>();

				math::Matrix<float, 4> MVP = t_mat * (1.0f/camera_transform->scale).to_scale_matrix() *
				(-camera_transform->position).to_translate_matrix<COLUMN_MAJOR>();

				glUniformMatrix4fv(mvp, 1, GL_FALSE, &MVP.data[0][0]);
				//glUniformMatrix4fv(scale, 1, GL_FALSE, &final_s_mat.data[0][0]);
				glDrawArrays(GL_LINES, 0, renderer->vertices.size() / 3);
			}
		}
	};
}
