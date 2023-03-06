#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

namespace P1::events {
	template <typename T, typename ...Args>
	class EventManager {
	private:
		typedef std::function<void(Args...)> FUNC;
		typedef std::vector<FUNC> EVENTS;
		typedef std::function<void(T, Args...)> FUNNEL;

		std::unordered_map<T, EVENTS> events{};
		std::vector<FUNNEL> funnels{};
	public:
		// Listen for event
		void on(T key, FUNC callback) {
			// Fetch event for specified key
			auto event = events.find(key);

			// Check if event exists
			if(event != events.end()) {
				// Add to existant array of callbacks
				(event->second).push_back(callback);
			}
			else {
				// Create a new array of callbacks and add it
				EVENTS listeners{callback};
				events.insert(std::pair<T, EVENTS>(key, listeners));
			}
		}

		// Emit an event
		void emit(T key, Args... args) {
			// Fetch event for specified key
			auto event = events.find(key);

			// Check if event exists
			if(event != events.end())
				// Run every callback for that event
				for(FUNC callback : event->second)
					callback(std::forward<Args>(args)...);

			// Run every void callback either the event exists or not
			for(FUNNEL callback : funnels)
				callback(std::forward<T>(key), std::forward<Args>(args)...);
		}

		// Listen for ANY event
		void funnel(FUNNEL callback) {
			funnels.push_back(callback);
		}
	};

	template <typename... Args>
	class EventManager<void, Args...> {
	private:
		#define FUNC std::function<void(Args...)>

		std::vector<FUNC> events{};
	public:
		void on(FUNC callback) {
			events.push_back(callback);
		}

		void emit(Args... args) {
			for(FUNC callback : events)
				callback(std::forward<Args>(args)...);
		}
	};
}
