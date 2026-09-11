#pragma once

#include <array>
#include <libtcod.hpp>
#include <optional>
#include <string_view>

class WindowFrame {
 public:
  WindowFrame(int x, int y, int width, int height, bool doubleFrame = false)
      : x(x), y(y), width(width), height(height), doubleFrame(doubleFrame) {}

  void setTitle(std::string_view t) { this->title = t; }
  void setFooter(std::string_view f) { this->footer = f; }

  std::array<int, 4> draw(tcod::Console& console) const;

 private:
  int x, y, width, height;
  bool doubleFrame = false;
  std::optional<std::string> title;
  std::optional<std::string> footer;
};
