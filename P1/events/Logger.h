#pragma once

#include <string>
#include <iostream>
#include <memory>
#include "EventManager.h"
#include "TerminalStyle.h"

namespace P1::events {
	#define DEBUG 0
	#define LOG 1
	#define WARNING 2
	#define CRITICAL 3
	#define ERROR 4

	#define LOG_NONE 0
	#define LOG_ONLY_DEBUG 1
	#define LOG_ONLY_LOG 2
	#define LOG_ONLY_WARNING 4
	#define LOG_ONLY_CRITICAL 8
	#define LOG_ONLY_ERROR 16
	#define LOG_ALL_BUT_DEBUG 30
	#define LOG_ALL_BUT_LOG 29
	#define LOG_ALL_BUT_WARNING 27
	#define LOG_ALL_BUT_CRITICAL 23
	#define LOG_ALL_BUT_ERROR 15
	#define LOG_ALL 31

	class Logger {
		std::string name;

	public:
		unsigned int logging_mask = LOG_ALL;
		const std::unique_ptr<EventManager<unsigned int, std::string>> event_manager =
			std::make_unique<EventManager<unsigned int, std::string>>();

	private:
		std::string get_name_lbl();

		std::string get_debug_lbl();
		std::string get_warn_lbl();
		std::string get_crit_lbl();
		std::string get_error_lbl();

	public:
		Logger(std::string name);
		Logger(std::string name, unsigned int mask);

	private:
		void init_events();
		
	public:
		void debug(std::string msg);
		void log(std::string msg);
		void warn(std::string msg);
		void crit(std::string msg);
		void error(std::string msg);
	};
}
