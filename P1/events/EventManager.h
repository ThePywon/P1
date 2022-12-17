#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <functional>

namespace P1 { namespace events {
	typedef std::function<void()> Callback;

	class VoidManager;
	class VoidListener {
	private:
		boost::uuids::uuid tag;
	protected:
		friend VoidManager;
		Callback callback;
	public:
		VoidListener(Callback callback)
			: tag(boost::uuids::random_generator()()) {
			this->callback = callback;
		}

		inline bool operator == (const VoidListener &other) {
			return tag == other.tag;
		}
	};

	template <typename T>
	class EventManager;
	template <typename T>
	class VoidRelay {
	private:
		boost::uuids::uuid tag;
	protected:
		friend EventManager<T>;
		std::vector<T> out;
	public:
		VoidRelay(std::vector<T> out) : tag(boost::uuids::random_generator()()) {
			this->out = out;
		}
		VoidRelay(T out) : tag(boost::uuids::random_generator()()) {
			this->out.push_back(out);
		}

		inline bool operator == (const VoidRelay &other) {
			return tag == other.tag;
		}
	};

	class VoidManager {
	private:
		std::vector<VoidListener> voidEvents;
	public:
		VoidListener pong(Callback callback) {
			VoidListener result(callback);
			voidEvents.push_back(result);
			return result;
		}

		void ping() {
			for(std::vector<VoidListener>::iterator v = voidEvents.begin(); v != voidEvents.end(); ++v)
				v->callback();
		}

		void remove(VoidListener listener) {
			auto match = std::find(voidEvents.begin(), voidEvents.end(), listener);
			if(match != voidEvents.end()) voidEvents.erase(match);
		}
	};

	template <typename T>
	class EventListener : public VoidListener {
	private:
		friend EventManager<T>;
		T key;
	public:
		EventListener(T key, Callback callback) : VoidListener(callback) {
			this->key = key;
			this->callback = callback;
		}
	};

	template <typename T>
	class Relay : public VoidRelay<T> {
	private:
		friend EventManager<T>;
		std::vector<T> in;
	public:
		Relay(T in, T out) : VoidRelay<T>(out) {
			this->in.push_back(in);
		}
		Relay(T in, std::vector<T> out) : VoidRelay<T>(out) {
			this->in.push_back(in);
		}
		Relay(std::vector<T> in, T out) : VoidRelay<T>(out) {
			this->in = in;
		}
		Relay(std::vector<T> in, std::vector<T> out) : VoidRelay<T>(out) {
			this->in = in;
		}
	};

	template <typename T>
	class EventManager : public VoidManager {
	private:
		std::map<T, std::vector<EventListener<T>>> events;
		std::vector<VoidRelay<T>> voidRelays;
		std::vector<Relay<T>> relays;
	public:
		EventListener<T> on(T key, Callback callback) {
			auto event = events.find(key);
			EventListener result(key, callback);

			if(event != events.end())
				(event->second).push_back(result);
			else {
				std::vector<EventListener<T>> listeners;
				listeners.push_back(result);
				events.insert(std::pair<T, std::vector<EventListener<T>>>(key, listeners));
			}

			return result;
		}
		void emit(T key) {
			auto event = events.find(key);
			if(event != events.end()) {
				std::vector<EventListener<T>> listeners = event->second;
				for(typename std::vector<EventListener<T>>::iterator e = listeners.begin(); e != listeners.end(); ++e)
					e->callback();
			}
			for(typename std::vector<VoidRelay<T>>::iterator v = voidRelays.begin(); v != voidRelays.end(); ++v)
				for(typename std::vector<T>::iterator t = v->out.begin(); t != v->out.end(); ++t) emit(*t);
		}

		#define ADD_VOID_RELAY voidRelays.push_back(VoidRelay<T>(out));
		#define ADD_RELAY relays.push_back(Relay<T>(in, out));
		void relay(T out) { ADD_VOID_RELAY }
		void relay(std::vector<T> out) { ADD_VOID_RELAY }
		void relay(T in, T out) { ADD_RELAY }
		void relay(T in, std::vector<T> out) { ADD_RELAY }
		void relay(std::vector<T> in, T out) { ADD_RELAY }
		void relay(std::vector<T> in, std::vector<T> out) { ADD_RELAY }


		void remove(EventListener<T> listener) {
			auto event = events.find(listener.key);
			if(event != events.end()) {
				auto match = std::find((event->second).begin(), (event->second).end(), listener);
				if(match != (event->second).end())
					(event->second).erase(match);
			}
		}
		void remove(VoidRelay<T> relay) {
			auto match = std::find(voidRelays.begin(), voidRelays.end(), relay);
			if(match != voidRelays.end())
				voidRelays.erase(match);
		}
		void remove(Relay<T> relay) {
			auto match = std::find(relays.begin(), relays.end(), relay);
			if(match != relays.end())
				relays.erase(match);
		}
	};
}}
