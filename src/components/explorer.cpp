#include "explorer.h"

#include "components/explorable.h"
#include "core/gameMap.h"

void Explorer::addMap(GameMap& map) {
  this->visitedMaps.emplace(map.getID(), ExploredGrid());
  ExploredGrid& grid = this->visitedMaps.at(map.getID());
  grid.height = map.getHeight();
  grid.width = map.getWidth();
  grid.visible = std::vector<bool>(grid.height * grid.width, false);
  grid.explored = std::vector<bool>(grid.height * grid.width, false);

  this->currentMap = map.getID();
  this->updateFOV(map);
}

void Explorer::setIsVisible(Position pos, bool isVisible) {
  // Get the vector
  ExploredGrid& grid = this->getCurrentMap();

  // First, set explored if possible
  int loc = GameMap::getLocInTileVector(pos, grid.width);
  if (isVisible) {
    grid.explored[loc] = true;
  }
  grid.visible[loc] = isVisible;
}

bool Explorer::isTileVisible(Position pos) {
  ExploredGrid& grid = this->getCurrentMap();
  return grid.visible[GameMap::getLocInTileVector(pos, grid.width)];
}

bool Explorer::hasExploredEntity(IDGenerator::ID entityID) {
  ExploredGrid& grid = this->getCurrentMap();
  return grid.discoveredEntities.contains(entityID);
}

Position Explorer::getDiscoveredEntityLastPosition(IDGenerator::ID entityID) {
  ExploredGrid& grid = this->getCurrentMap();
  return grid.discoveredEntities.at(entityID);
}

std::vector<Entity*> visibleEntities(GameMap& map, Explorer& explorer, Entity* self) {
  auto& grid = explorer.getMap(map.getID());
  std::vector<Entity*> result;
  for (const auto& e : map.getEntities()) {
    if (e.get() == self) continue;
    Position p = e->getPosition();
    if (map.inBounds(p) && grid.visible[GameMap::getLocInTileVector(p, grid.width)]) result.push_back(e.get());
  }
  return result;
}

void Explorer::updateFOV(GameMap& map) {
  int maxRadius = 0;  // TODO: If >0, this is how far the player can see
  TCODMap& fovMap = map.getFOVMap();
  fovMap.computeFov(this->getEntity()->getPosition().x, this->getEntity()->getPosition().y, maxRadius, true, FOV_BASIC);

  // Update the isVisible map with FOV information
  ExploredGrid& grid = this->getCurrentMap();
  for (int x = 0; x < grid.width; x++) {
    for (int y = 0; y < grid.height; y++) {
      this->setIsVisible(Position(x, y), fovMap.isInFov(x, y));
    }
  }

  // Update explorable entities
  for (auto& e : map.getEntities()) {
    if (e->hasComponent<Explorable>() && this->isTileVisible(e->getPosition())) {
      grid.discoveredEntities.insert_or_assign(e->getID(), e->getPosition());
    }
  }
}
