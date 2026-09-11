#pragma once

#include <libtcod/color.hpp>
#include <libtcod/console_types.hpp>

struct Tile {
  bool isWalkable;
  bool isTransparent;
  bool isWall;
  TCOD_ConsoleTile darkGraphic;
  TCOD_ConsoleTile lightGraphic;
};

Tile createNewTile(
    bool isWalkable, bool isTransparent, bool isWall, TCOD_ConsoleTile darkGraphic, TCOD_ConsoleTile lightGraphic);

TCOD_ConsoleTile getShroud();

Tile createWallTile();

Tile createFloorTile();
