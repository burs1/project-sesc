#include "ini_parser/ini_parser.h"
#include <fstream>
#include <iostream>

IniParser::IniParser() {
    // Constructor implementation
}

IniParser::~IniParser() {
    // Destructor implementation
}

void IniParser::parseIniFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::string currentSection;

    while (std::getline(file, line)) {
        if (line[0] == '[' && line[line.size()-1] == ']') {
            currentSection = line.substr(1, line.size()-2);
            iniData[currentSection] = std::unordered_map<std::string, std::string>();
        } else {
            size_t equalsPos = line.find('=');
            if (equalsPos != std::string::npos) {
                std::string key = line.substr(0, equalsPos);
                std::string value = line.substr(equalsPos + 1);
                iniData[currentSection][key] = value;
            }
        }
    }
}

void IniParser::printIniData() {
    for (const auto& section : iniData) {
        std::cout << "AWDASDAWDASDAWDASD";
        std::cout << "[" << section.first << "]" << std::endl;
        for (const auto& option : section.second) {
            std::cout << option.first << " = " << option.second << std::endl;
        }
    }
}