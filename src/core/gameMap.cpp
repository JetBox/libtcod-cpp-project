#include "gameMap.h"

#include <libtcod/console.h>

#include <memory>
#include <vector>

#include "common/idGenerator.h"
#include "common/tiles.h"
#include "components/explorable.h"
#include "components/explorer.h"

GameMap::GameMap(IDGenerator::ID id, int width, int height) {
  this->id = id;
  this->width = width;
  this->height = height;
  int size = width * height;
  this->tiles = std::vector<Tile>(size, createWallTile());

  // Initialize Entities to empty
  this->entities = std::vector<std::unique_ptr<Entity>>();
}

bool GameMap::inBounds(Position pos) {
  return (0 <= pos.x && pos.x < this->width) && (0 <= pos.y && pos.y < this->height);
}

void GameMap::render(
    tcod::Console* console, Entity* player, Position camera, int viewX, int viewY, int viewH, int viewW) {
  for (int sy = 0; sy < viewH; ++sy) {
    for (int sx = 0; sx < viewW; ++sx) {
      // Screen (sx,sy) maps to world (wx,wy)
      int wx = sx + camera.x;
      int wy = sy + camera.y;

      // Skip anything outside the actual map (only happens if unclamped)
      if (wx < 0 || wy < 0 || wx >= this->width || wy >= this->height) continue;

      int loc = this->getLocInTileVector(wx, wy);
      TCOD_ConsoleTile drawGraphic;
      Explorer& explorer = player->getComponent<Explorer>();
      if (!explorer.getCurrentMap().explored[loc]) {
        drawGraphic = getShroud();
      } else if (!explorer.getCurrentMap().visible[loc]) {
        drawGraphic = this->tiles[loc].darkGraphic;
      } else {
        drawGraphic = this->tiles[loc].lightGraphic;
      }
      console->at({sx + viewX, sy + viewY}) = drawGraphic;  // note: SCREEN coords here
    }
  }

  // Draw in render-order
  std::vector<Entity*> drawList;
  drawList.reserve(this->entities.size());
  for (const auto& e : this->entities) {
    drawList.push_back(e.get());
  }
  std::sort(
      drawList.begin(), drawList.end(), [](Entity* a, Entity* b) { return a->getRenderOrder() < b->getRenderOrder(); });

  for (const auto& e : drawList) {
    if (!player->getComponent<Explorer>().isTileVisible(e->getPosition())) {
      // Entities with the Explorable may be drawn
      if (e->hasComponent<Explorable>() && e->getComponent<Explorable>().canDrawToPlayer() &&
          player->getComponent<Explorer>().hasExploredEntity(e->getID())) {
        // Position screenPos = e->getPosition() - camera;  // world - camera
        Position screenPos = player->getComponent<Explorer>().getDiscoveredEntityLastPosition(e->getID());
        screenPos -= camera;
        if (screenPos.x < 0 || screenPos.y < 0 || screenPos.x >= viewW || screenPos.y >= viewH) {
          continue;
        }
        TCOD_ConsoleTile& tile = console->at({screenPos.x + viewX, screenPos.y + viewY});
        tile.ch = e->getChar();
        tile.fg = e->getComponent<Explorable>().getDimColor();
      }
    } else {
      Position screenPos = e->getPosition() - camera;  // world - camera
      if (screenPos.x < 0 || screenPos.y < 0 || screenPos.x >= viewW || screenPos.y >= viewH) {
        continue;
      }
      TCOD_ConsoleTile& tile = console->at({screenPos.x + viewX, screenPos.y + viewY});
      tile.ch = e->getChar();
      tile.fg = e->getColor();
    }
  }
}

Tile GameMap::getTileAt(Position pos) { return this->tiles[this->getLocInTileVector(pos)]; }

Tile GameMap::getTileAt(int x, int y) { return this->tiles[this->getLocInTileVector(x, y)]; }

void GameMap::carveFloor(const Position pos) { this->tiles[this->getLocInTileVector(pos)] = createFloorTile(); }

int GameMap::getLocInTileVector(Position pos) { return GameMap::getLocInTileVector(pos.x, pos.y, this->width); }

int GameMap::getLocInTileVector(int x, int y) { return GameMap::getLocInTileVector(x, y, this->width); }

int GameMap::getLocInTileVector(Position pos, int width) { return GameMap::getLocInTileVector(pos.x, pos.y, width); }
int GameMap::getLocInTileVector(int x, int y, int width) { return (y * width) + x; }

void GameMap::addEntity(std::unique_ptr<Entity> e) { this->entities.push_back(std::move(e)); }

Entity* GameMap::getBlockingEntity(Position p) {
  for (const auto& e : this->entities) {
    if (e->getBlocksMovement() && e->getPosition() == p) {
      return e.get();
    }
  }
  return nullptr;
}

bool GameMap::hasVisionBlockingEntity(Position p) {
  for (const auto& e : this->entities) {
    if (e->getPosition() == p && e->getBlocksVision()) {
      return true;
    }
  }
  return false;
}

std::vector<Entity*> GameMap::getEntitiesAt(Position p, Entity* exclude) {
  std::vector<Entity*> eList;
  for (const auto& e : this->entities) {
    if (e->getPosition() == p && e.get() != exclude) {
      eList.push_back(e.get());
    }
  }
  return eList;
}

std::unique_ptr<Entity> GameMap::extractEntity(Entity* target) {
  for (auto it = entities.begin(); it != entities.end(); ++it) {
    if (it->get() == target) {
      std::unique_ptr<Entity> owned = std::move(*it);
      entities.erase(it);
      return owned;
    }
  }
  return nullptr;  // not found
}

TCODMap& GameMap::getFOVMap() {
  if (!this->fovMap) {
    this->fovMap = std::make_unique<TCODMap>(this->width, this->height);
    this->buildFOVMap();
  }
  return *this->fovMap;
}

TCODPath& GameMap::getPathfinder() {
  if (!this->pathfinder) {
    this->pathfinder = std::make_unique<TCODPath>(&this->getFOVMap(), 0.0f);
  }
  return *this->pathfinder;
}

void GameMap::buildFOVMap() {
  for (int x = 0; x < this->width; ++x) {
    for (int y = 0; y < this->height; ++y) {
      this->updateFOVCell(Position(x, y));
    }
  }
}

void GameMap::updateFOVCell(Position pos) {
  if (!this->fovMap) return;
  Tile t = this->getTileAt(pos);
  bool transparent = t.isTransparent && !this->hasVisionBlockingEntity(pos);
  this->fovMap->setProperties(pos.x, pos.y, transparent, t.isWalkable);
}
