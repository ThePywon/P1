#include "Logger.h"

namespace P1::events {
	Logger::Logger(std::string name) {
		this->name = name;

		init_events();
	}
	Logger::Logger(std::string name, unsigned int mask) {
		this->name = name;
		logging_mask = mask;

		init_events();
	}

	void Logger::init_events() {
		if(1 << DEBUG & this->logging_mask)
      event_manager->on(DEBUG, [this](std::string msg) {
        std::cout << this->get_name_lbl() << " " << this->get_debug_lbl() << " " << msg << std::endl;
      });
		if(1 << LOG & this->logging_mask)
      event_manager->on(LOG, [this](std::string msg) {
        std::cout << this->get_name_lbl() << " " << msg << std::endl;
      });
    if(1 << WARNING & this->logging_mask)
      event_manager->on(WARNING, [this](std::string msg) {
        std::cout << this->get_name_lbl() << " " << this->get_warn_lbl() << " " << msg << std::endl;
      });
    if(1 << CRITICAL & this->logging_mask)
      event_manager->on(CRITICAL, [this](std::string msg) {
        std::cout << this->get_name_lbl() << " " << this->get_crit_lbl() << " " << msg << std::endl;
      });
    if(1 << ERROR & this->logging_mask)
      event_manager->on(ERROR, [this](std::string msg) {
        std::cout << this->get_name_lbl() << " " << this->get_error_lbl() << " " << msg << std::endl;
      });
	}

	std::string Logger::get_name_lbl() {
		return terminal_style::set_style({LIGHT_WHITE, BLACK_BG}, "[" + this->name + "]");
	}

	std::string Logger::get_debug_lbl() {
		return terminal_style::set_style({LIGHT_CYAN, BLACK_BG}, "<DEBUG>");
	}
	std::string Logger::get_warn_lbl() {
		return terminal_style::set_style({LIGHT_YELLOW, BLACK_BG}, "<WARNING>");
	}
	std::string Logger::get_crit_lbl() {
		return terminal_style::set_style({MAGENTA, BOLD, BLACK_BG}, "<CRITICAL>");
	}
	std::string Logger::get_error_lbl() {
		return terminal_style::set_style({RED, BOLD, BLACK_BG}, "<ERROR>");
	}

	void Logger::debug(std::string msg) {
		event_manager->emit(DEBUG, msg);
	}
	void Logger::log(std::string msg) {
		event_manager->emit(LOG, msg);
	}
	void Logger::warn(std::string msg) {
		event_manager->emit(WARNING, msg);
	}
	void Logger::crit(std::string msg) {
		event_manager->emit(CRITICAL, msg);
	}
	void Logger::error(std::string msg) {
		event_manager->emit(ERROR, msg);
	}
  void Logger::debug_sync(std::string msg) {
		event_manager->emit_sync(DEBUG, msg);
	}
	void Logger::log_sync(std::string msg) {
		event_manager->emit_sync(LOG, msg);
	}
	void Logger::warn_sync(std::string msg) {
		event_manager->emit_sync(WARNING, msg);
	}
	void Logger::crit_sync(std::string msg) {
		event_manager->emit_sync(CRITICAL, msg);
	}
	void Logger::error_sync(std::string msg) {
		event_manager->emit_sync(ERROR, msg);
	}
}
