#pragma once

#include <string>
#include <unordered_map>

class IniParser {
public:
  IniParser();
  ~IniParser();

  // Parse the INI file
  auto parseIniFile(const std::string& filename) -> void;

  // Print the parsed INI data
  auto printIniData() -> void;

private:
  std::unordered_map<std::string, std::unordered_map<std::string, std::string>> iniData;
};