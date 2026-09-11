#pragma once

#include <unordered_map>
#include <unordered_set>

#include "common/idGenerator.h"
#include "common/position.h"
#include "component.h"
#include "core/gameMap.h"

struct ExploredGrid {
  int width = 0, height = 0;
  std::vector<bool> explored, visible;
  std::unordered_map<IDGenerator::ID, Position> discoveredEntities;  // Position is where last seen
};

class Explorer : public BaseComponent {
 public:
  Explorer(Entity* owner) { this->setEntity(owner); }  // Explorer NEEDS a reference to it's owner

  void addMap(GameMap& map);
  void setCurrentMap(IDGenerator::ID mapID) { this->currentMap = mapID; }
  IDGenerator::ID getCurrentMapID() { return this->currentMap; }
  ExploredGrid& getCurrentMap() { return this->visitedMaps[this->currentMap]; }
  ExploredGrid& getMap(IDGenerator::ID mapID) { return this->visitedMaps.at(mapID); }

  bool hasExploredEntity(IDGenerator::ID entityID);
  Position getDiscoveredEntityLastPosition(IDGenerator::ID entityID);

  void setIsVisible(Position pos, bool isVisible);
  bool isTileVisible(Position pos);

  void updateFOV(GameMap& map);

 private:
  IDGenerator::ID currentMap = 0;
  std::unordered_map<IDGenerator::ID, ExploredGrid> visitedMaps;
};

std::vector<Entity*> visibleEntities(GameMap& map, Explorer& explorer, Entity* self);
