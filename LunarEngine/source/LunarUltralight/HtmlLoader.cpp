#include "../../public/LunarUltralight/HtmlLoader.h"
#include <streambuf>
#include <fstream>


namespace LunarGUI {
	HTMLLoader::HTMLLoader() {

	}

	HTMLLoader::HTMLLoader(const std::string& fileLoc) {
		this->fileLoc = fileLoc;
		std::ifstream file("html/" + fileLoc);

		if(!file.is_open())
			throw std::runtime_error("couldnt open the file");

		file.seekg(0, std::ios::end);
		unparsedHTML.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		unparsedHTML.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		parse();
	}

	HTMLLoader::~HTMLLoader() {

	}

	std::string HTMLLoader::HTMLContents() {
		return htmlContents;
	}

	void HTMLLoader::parse() {
		//split unparsed into vector of stirngs
		std::vector<std::string> unparsed_strings = split(unparsedHTML, '\n');

		int locksFound = 0;

		//locks
		bool foundStylesheet = false;
		std::string stylesheet_loc;

		bool foundScript = false;
		std::string script_loc;

		for (auto line : unparsed_strings) {
			char c = line.at(0);
			if (c == '@') {
				std::vector<std::string> args = split(line, ' ');

				if (args[0] == "@import" && args[1] == "stylesheet") {
					stylesheet_loc = args[2];
					stylesheet_loc.pop_back();

					foundStylesheet = true;
					locksFound += 1;
				}
				else if (args[0] == "@import" && args[1] == "script") {
					script_loc = args[2];
					script_loc.pop_back();

					foundScript = true;
					locksFound += 1;
				}
			}
		}

		if (foundStylesheet) {
			std::vector<std::string> nameArgs = split(this->fileLoc, '.');
			std::string fileName = nameArgs[0];

			std::string styleLoc = "html/" + fileName + "-res/styles.css";
			std::string styleSheetContents;

			std::ifstream file(styleLoc);

			if (!file.is_open())
				throw std::runtime_error("couldnt open the file");

			file.seekg(0, std::ios::end);
			styleSheetContents.reserve(file.tellg());
			file.seekg(0, std::ios::beg);

			styleSheetContents.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());

			bool inserted = false;
			for (int i = 0; i < unparsed_strings.size() && !inserted; i++) {
				std::string line = unparsed_strings[i];
				line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

				if (line == "<head>") {
					std::string styleNode = "<style>" + styleSheetContents + "</style>";
					unparsed_strings.insert(unparsed_strings.begin() + i + 1, styleNode);
					inserted = true;
				}
			}
		}

		if (foundScript) {
			std::vector<std::string> nameArgs = split(this->fileLoc, '.');
			std::string fileName = nameArgs[0];

			std::string styleLoc = "html/" + fileName + "-res/index.js";
			std::string scriptContents;

			std::ifstream file(styleLoc);

			if (!file.is_open())
				throw std::runtime_error("couldnt open the file");

			file.seekg(0, std::ios::end);
			scriptContents.reserve(file.tellg());
			file.seekg(0, std::ios::beg);

			scriptContents.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());

			bool inserted = false;
			for (int i = 0; i < unparsed_strings.size() && !inserted; i++) {
				std::string line = unparsed_strings[i];
				line.erase(std::remove_if(line.begin(), line.end(), std::isspace), line.end());

				if (line == "</body>") {
					std::string styleNode = "<script>" + scriptContents + "</script>";
					unparsed_strings.insert(unparsed_strings.begin() + i, styleNode);
					inserted = true;
				}
			}
		}

		//cleanup
		for (int i = 0; i < locksFound; i++) {
			unparsed_strings.erase(unparsed_strings.begin());
		}

		for (auto line : unparsed_strings) {
			htmlContents += line;
		}
	}

	std::vector<std::string> HTMLLoader::split(const std::string& str, char delim) {
		std::vector<std::string> strings;

		size_t start;
		size_t end = 0;

		while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
			end = str.find(delim, start);
			strings.push_back(str.substr(start, end - start));
		}
		
		return strings;
	}
}