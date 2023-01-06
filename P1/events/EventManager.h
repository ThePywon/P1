#pragma once

#include <vector>
#include <map>
#include <functional>

namespace P1::events {
	#define FUNC(A) std::function<void(A)>
	#define EVENTS(A) std::vector<FUNC(A)>

	template <typename T, typename ...Args>
	class EventManager {
	private:
		std::map<T, EVENTS(Args...)> events{};
		EVENTS(Args...) void_events{};
	public:
		void on(T key, FUNC(Args...) callback) {
			auto event = events.find(key);

			if(event != events.end())
				(event->second).push_back(callback);
			else {
				EVENTS(Args...) listeners{callback};
				events.insert(std::pair<T, EVENTS(Args...)>(key, listeners));
			}
		}

		void emit(T key) {
			auto event = events.find(key);

			if(event != events.end())
				for(FUNC(Args...) callback : event->second) callback();

			for(FUNC(Args...) callback : void_events) callback();
		}

		void funnel(FUNC(Args...) callback) {
			void_events.push_back(callback);
		}
	};
}
