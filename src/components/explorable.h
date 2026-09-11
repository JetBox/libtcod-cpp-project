#pragma once

#include <libtcod/color.hpp>

#include "component.h"

class Explorable : public BaseComponent {
 public:
  Explorable(tcod::ColorRGBA dimColor, bool drawToPlayer = false) : dimColor(dimColor), drawToPlayer(drawToPlayer) {}

  tcod::ColorRGBA getDimColor() { return this->dimColor; }

  bool canDrawToPlayer() { return this->drawToPlayer; }

 private:
  tcod::ColorRGBA dimColor = {0, 0, 0, 255};
  bool drawToPlayer = false;
};
