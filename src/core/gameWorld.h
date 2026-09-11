#pragma once

#include <memory>
#include <unordered_map>

#include "common/idGenerator.h"
#include "gameMap.h"

class GameWorld {
 public:
  GameWorld() = default;

  IDGenerator::ID getActiveMapID() { return this->activeMap; }
  GameMap& getMap(IDGenerator::ID mapID);
  GameMap& getActiveMap() { return this->getMap(this->activeMap); }
  void setActiveMap(IDGenerator::ID mapID) { this->activeMap = mapID; }

  void addEntityToMap(std::unique_ptr<Entity> entity, IDGenerator::ID mapID = 0);

  IDGenerator::ID createMap(int width, int height);

 private:
  IDGenerator gameMapIDGenerator;
  IDGenerator entityIDGenerator;
  std::unordered_map<IDGenerator::ID, GameMap> maps;  // Map of ID -> GameMap
  IDGenerator::ID activeMap = 0;
};
