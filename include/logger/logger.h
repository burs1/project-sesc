#pragma once 

#include <string>

#include <spdlog/spdlog.h>

namespace eng::log {

inline auto Info(std::string text)  -> void {
  spdlog::info(text);
}


inline auto Warn(std::string text)  -> void {
  spdlog::warn(text);
}


inline auto Error(std::string text) -> void {
  spdlog::error(text);
}


}
