#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <optional>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <array>

namespace LunarGUI {
	class HTMLLoader {
	public:
		HTMLLoader();
		HTMLLoader(const std::string& fileLoc);
		~HTMLLoader();

		std::string HTMLContents();
	private:
		std::string unparsedHTML;
		std::string htmlContents;
		std::string fileLoc;

		void parse();

		//utilities
		std::vector<std::string> split(const std::string& str, char delim);
	};
}