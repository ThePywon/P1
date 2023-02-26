#pragma once

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <thread>
#include <assert.h>
#include "../concepts/GameConcepts.h"
#include "../events/EventManager.h"
#include "../systems/MainManager.h"
#include "../graphics/Material.h"
#include "../components/Component.h"

namespace P1::graphics { class Material; }

namespace P1::systems { class MainManager; }

namespace P1::entity {
	// Entity id generator method declaration
	extern unsigned int entity_id_counter;
	extern std::vector<unsigned int> freed_ids;
	unsigned int get_entity_id();

	class Scene {
	private:
		// Store active and destroyed entities
		friend Entity;
		friend systems::MainManager;
		std::vector<std::shared_ptr<Entity>> entities{};
		std::vector<std::pair<unsigned int, unsigned int>> garbage{};

		// Store component pools
		std::unordered_map<unsigned int, std::shared_ptr<components::ComponentPoolBase>> component_pools;

	public:
		events::EventManager<Entity*> event_manager{};

		Scene() {
			systems::MainManager::scenes.push_back(this);
		}

	private:
		// Get pool from type
		template <concepts::Component T>
		std::shared_ptr<components::ComponentPool<T>> get_pool() {
			// Garbage collect component type
			garbage_collect<T>();

			// Get component id and base component pool
			unsigned int id = components::get_component_id<T>();
			auto base_pool = component_pools[id];

			// Check if pool exists
			if(base_pool)
				// If so, convert to specialized pool and return it
				return std::static_pointer_cast<components::ComponentPool<T>>(base_pool);

			// Otherwise create a specialized pool and store as base pool then return it
			auto pool = std::make_shared<components::ComponentPool<T>>();
			component_pools[id] = std::static_pointer_cast<components::ComponentPoolBase>(pool);
			return pool;
		}

		// Garbage collect a specific type of component
		template <concepts::Component T>
		void garbage_collect() {
			// Get component id and bitfield values
			unsigned int id = components::get_component_id<T>();
			unsigned int bit = 1 << id;
			// New array to store any sorted garbage
			std::vector<std::vector<std::pair<unsigned int, unsigned int>>::iterator> sorted_garbage{};

			// Loop over garbage
			for(auto pair = garbage.begin(); pair != garbage.end(); ++pair) {
				// Make sure the component type matches the garbage
				if(!(pair->second & bit)) continue;

				// Get the corresponding component pool
				auto pool = std::static_pointer_cast<components::ComponentPool<T>>(component_pools[id]);
				// Loop over the pool
				for(auto it = pool->components.begin(); it != pool->components.end(); ++it) {
					// Make sure the specific component matches the garbage
					if((*it)->entity_id != pair->first) continue;

					// Erase the component
					pool->components.erase(it);
					// Check if the garbage matches EXACTLY the component type
					if(pair->second == bit)
						// If so, add to the sorted garbage array
						sorted_garbage.push_back(pair);
					// Flip the garbage's corresponding component mask bit (entities cannot contain more than one component of a specific type)
					pair->second ^= bit;
					// Match found, stop looping
					break;
				}
			}

			// Loop over sorted garbage
			for(auto it : sorted_garbage) {
				// Free corresponding entity id
				freed_ids.push_back(it->first);
				// Remove from the garbage pile
				garbage.erase(it);
			}
		}

	public:
		~Scene() {
			for(auto it = systems::MainManager::scenes.begin(); it != systems::MainManager::scenes.end(); ++it)
				if(*it == this) {
					systems::MainManager::scenes.erase(it);
					return;
				}
		}
	};

	class Entity : public std::enable_shared_from_this<Entity> {
	private:
		unsigned int id;

		// Pointer to parent scene
		Scene* scene;

		// Component bitfield
		unsigned int component_mask{0};

		// Creation assertion struct
		struct use_create_method {
			explicit use_create_method() = default;
		};
	public:
		std::string name;
		
		std::vector<std::shared_ptr<graphics::Material>> materials{};

		Entity(use_create_method assertion, const std::string& name, Scene* scene) {
			this->name = name;
			this->scene = scene;
			// Get id from generator method
			id = get_entity_id();
			std::cout << "Entity \"" << name << "\" #" << id << " created." << std::endl;
		}

		// Forced alternative constructor
		static Entity* create(Scene* scene, const std::string& name);

		inline std::string get_name() { return name; }

		// Remove entity from scene
		void destroy() {
			for(auto it = scene->entities.begin(); it != scene->entities.end(); ++it)
				if((*it)->id == id) {
					scene->entities.erase(it);
					scene->garbage.push_back({id, component_mask});
					return;
				}
		}

		template <concepts::Component T>
		bool has_component() {
			return component_mask & 1 << components::get_component_id<T>();
		}

		template <concepts::Component T>
		T* add_component() {
			// Get component type id and bitfield values
			unsigned int id = components::get_component_id<T>();
			unsigned int bit = 1 << id;
			// Refuse operation if component is already detected
			if(component_mask & bit)
				return nullptr;

			// Create and initialize component
			std::shared_ptr<T> component = std::make_shared<T>();
			component->id = id;
			component->entity_id = this->id;
			// Add to corresponding component pool
			scene->get_pool<T>()->components.push_back(component);
			// Add to component mask bitfield
			component_mask |= bit;

			scene->event_manager.emit(this);

			return component.get();
		}

		template <concepts::Component T>
		T* get_component() {
			// Make sure the component is in the component mask
			if(!(component_mask & 1 << components::get_component_id<T>()))
				return nullptr;

			// Fetch corresponding component pool
			auto pool = scene->get_pool<T>();
			// Get matching component from pool and return it
			for(std::shared_ptr<T>& component : pool->components)
				if(component->entity_id == id)
					return component.get();

			// If nothing found, return nullptr
			return nullptr;
		}

		template <concepts::Component T>
		void remove_component() {
			unsigned int bit = 1 << components::get_component_id<T>();
			assert(component_mask & bit);

			std::shared_ptr<components::ComponentPool<T>> pool = scene->get_pool<T>();
			for(auto it = pool->components.begin(); it != pool->components.end(); ++it)
				if((*it)->entity_id == id) {
					pool->components.erase(it);
					break;
				}

			component_mask ^= bit;

			scene->event_manager.emit(this);
		}

		bool operator == (const Entity& other) {
			return id == other.id;
		}

		Entity (Entity& other) = delete;
	};
}
