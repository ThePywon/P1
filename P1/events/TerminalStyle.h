#pragma once

#include <string>
#include <vector>
#include <initializer_list>

namespace P1::events::terminal_style {
	#define RESET 0u
	#define BOLD 1u
	#define DARK 2u
	#define ITALIC 3u
	#define UNDERLINE 4u
	#define INVERSE 5u
	#define FLASHING 6u
	#define RESET_BOLD 21u
	#define RESET_DARK 22u
	#define RESET_ITALIC 23u
	#define RESET_UNDERLINE 24u
	#define RESET_INVERSE 27u
	#define RESET_FLASHING 28u

	#define BLACK 30u
	#define RED 31u
	#define GREEN 32u
	#define YELLOW 33u
	#define BLUE 34u
	#define MAGENTA 35u
	#define CYAN 36u
	#define WHITE 37u

	#define BLACK_BG 40u
	#define RED_BG 41u
	#define GREEN_BG 42u
	#define YELLOW_BG 43u
	#define BLUE_BG 44u
	#define MAGENTA_BG 45u
	#define CYAN_BG 46u
	#define WHITE_BG 47u

	#define GRAY 90u
	#define LIGHT_RED 91u
	#define LIGHT_GREEN 92u
	#define LIGHT_YELLOW 93u
	#define LIGHT_BLUE 94u
	#define LIGHT_MAGENTA 95u
	#define LIGHT_CYAN 96u
	#define LIGHT_WHITE 97u

	#define GRAY_BG 100u
	#define LIGHT_RED_BG 101u
	#define LIGHT_GREEN_BG 102u
	#define LIGHT_YELLOW_BG 103u
	#define LIGHT_BLUE_BG 104u
	#define LIGHT_MAGENTA_BG 105u
	#define LIGHT_CYAN_BG 106u
	#define LIGHT_WHITE_BG 107u

	inline std::string get_style(unsigned int style) {
		return "\033[" + std::to_string(style) + "m";
	}
	inline std::string get_style(std::vector<unsigned int> styles) {
		std::string s_styles = "";
		for(auto it = styles.begin(); it != styles.end(); ++it)
			s_styles += (it != styles.begin() ? ";" : "") + std::to_string(*it);
		return "\033[" + s_styles + "m";
	}
	inline std::string get_style(std::initializer_list<unsigned int> styles) {
		std::string s_styles = "";
		for(auto it = styles.begin(); it != styles.end(); ++it)
			s_styles += (it != styles.begin() ? ";" : "") + std::to_string(*it);
		return "\033[" + s_styles + "m";
	}

	inline std::string set_style(unsigned int style, std::string msg) {
		return get_style(style) + msg + get_style(RESET);
	}
	inline std::string set_style(std::vector<unsigned int> styles, std::string msg) {
		return get_style(styles) + msg + get_style(RESET);
	}
	inline std::string set_style(std::initializer_list<unsigned int> styles, std::string msg) {
		return get_style(styles) + msg + get_style(RESET);
	}
}
