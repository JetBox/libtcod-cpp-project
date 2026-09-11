#pragma once

#include <libtcod.hpp>
#include <libtcod/console_types.hpp>
#include <memory>

#include "common/entity.h"
#include "common/idGenerator.h"
#include "common/position.h"
#include "common/tiles.h"

class GameMap {
  friend class GameWorld;

 public:
  GameMap() = default;
  GameMap(IDGenerator::ID id, int width, int height);

  static int getLocInTileVector(Position pos, int width);
  static int getLocInTileVector(int x, int y, int width);

  IDGenerator::ID getID() { return this->id; }

  int getWidth() { return this->width; }

  int getHeight() { return this->height; }

  Position getOrigin() { return this->origin; }

  const std::vector<std::unique_ptr<Entity>>& getEntities() { return this->entities; }

  template <class T>
  std::vector<Entity*> getEntitiesWithComponent() {
    std::vector<Entity*> eList;
    for (auto& e : this->entities) {
      if (e->hasComponent<T>()) {
        eList.push_back(e.get());
      }
    }
    return eList;
  }

  void setOrigin(Position pos) { this->origin = pos; }

  bool inBounds(Position pos);

  void render(tcod::Console* console, Entity* player, Position camera, int viewX, int viewY, int viewH, int viewW);

  Tile getTileAt(Position pos);

  Tile getTileAt(int x, int y);

  void carveFloor(Position pos);

  void buildFOVMap();

  TCODMap& getFOVMap();

  TCODPath& getPathfinder();

  void updateFOVCell(Position pos);

  Entity* getBlockingEntity(Position p);

  bool hasVisionBlockingEntity(Position p);

  std::vector<Entity*> getEntitiesAt(Position p, Entity* exclude = nullptr);

  std::unique_ptr<Entity> extractEntity(Entity* target);

 private:
  IDGenerator::ID id;
  int width;
  int height;
  Position origin;

  // 2D Array stored as a single array
  std::vector<Tile> tiles;

  // FOV & Pathfinding
  std::unique_ptr<TCODMap> fovMap;
  std::unique_ptr<TCODPath> pathfinder;

  // Entities
  std::vector<std::unique_ptr<Entity>> entities;
  void addEntity(std::unique_ptr<Entity> e);

  int getLocInTileVector(Position pos);
  int getLocInTileVector(int x, int y);
};
