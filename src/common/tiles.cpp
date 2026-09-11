#include "tiles.h"

#include <libtcod/console.h>

#include <libtcod/color.hpp>

#include "colors.h"

Tile createNewTile(
    bool isWalkable, bool isTransparent, bool isWall, TCOD_ConsoleTile darkGraphic, TCOD_ConsoleTile lightGraphic) {
  Tile _return_val;
  _return_val.isWalkable = isWalkable;
  _return_val.isTransparent = isTransparent;
  _return_val.darkGraphic = darkGraphic;
  _return_val.lightGraphic = lightGraphic;
  _return_val.isWall = isWall;
  return _return_val;
}

TCOD_ConsoleTile getShroud() { return {' ', ENTITY_WHITE, SHROUD_BLACK}; }

Tile createFloorTile() {
  return createNewTile(true, true, false, {' ', ENTITY_WHITE, FLOOR_DARK}, {' ', ENTITY_WHITE, FLOOR_LIGHT});
}

Tile createWallTile() {
  return createNewTile(false, false, true, {' ', ENTITY_WHITE, WALL_DARK}, {' ', ENTITY_WHITE, WALL_LIGHT});
}
