#pragma once

#include <unordered_map>

enum class DebugOption { SHOW_ALL_ENTITIES, SHOW_ENTIRE_MAP };

class Debug {
 public:
  Debug() { this->debugOptions = {{DebugOption::SHOW_ALL_ENTITIES, false}, {DebugOption::SHOW_ENTIRE_MAP, false}}; }

  void toggleDebugOption(DebugOption option) { this->debugOptions.at(option) = !this->debugOptions.at(option); }
  bool getOption(DebugOption option) { return this->debugOptions.at(option); }

 private:
  std::unordered_map<DebugOption, bool> debugOptions;
};
