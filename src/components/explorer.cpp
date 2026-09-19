#include "explorer.h"

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "common/position.h"
#include "components/combat.h"
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

void Explorer::setIsVisible(ExploredGrid& grid, Position pos, bool isVisible) {
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

int Explorer::getFOVRadius() {
  // TODO: factor in stuff like torches/light spell
  return 5;
}

void Explorer::updateFOV(GameMap& map) {
  int maxRadius = this->getFOVRadius();  // TODO: If >0, this is how far the player can see
  TCODMap& fovMap = map.getFOVMap();
  fovMap.computeFov(this->getEntity()->getPosition().x, this->getEntity()->getPosition().y, maxRadius, true, FOV_BASIC);

  // Update the isVisible map with FOV information
  ExploredGrid& grid = this->getCurrentMap();
  for (int y = 0; y < grid.height; y++) {
    for (int x = 0; x < grid.width; x++) {
      this->setIsVisible(grid, Position(x, y), fovMap.isInFov(x, y));
    }
  }

  // Update explorable entities
  for (auto& e : map.getEntities()) {
    if (e->hasComponent<Explorable>() && this->isTileVisible(e->getPosition())) {
      grid.discoveredEntities.insert_or_assign(e->getID(), e->getPosition());
    }
  }
}

Position Explorer::getNearestFighterToPlayer(Engine& engine) {
  std::unordered_set<Position> exploredTiles;
  std::deque<Position> queuedPositions;

  Position currentPosition = this->getEntity()->getPosition();
  exploredTiles.emplace(currentPosition);
  ExploredGrid& grid = this->getCurrentMap();

  do {
    // Get the neighbors of the current position
    for (int i = 0; i < static_cast<int>(Direction::STATIONARY); i++) {
      Direction dir = static_cast<Direction>(i);
      Position newPos = currentPosition + dir;
      if (exploredTiles.contains(newPos)) {
        continue;
      }

      int loc = GameMap::getLocInTileVector(newPos, grid.width);

      // If unexplored or not visible, skip
      if (grid.explored[loc] && grid.visible[loc]) {
        // Get entity here
        std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(newPos);
        for (auto e : entities) {
          if (e->hasComponent<Fighter>()) {
            return e->getPosition();
          }
        }

        // If we're here, there was no entity. Add to queue and move on
        exploredTiles.emplace(newPos);
        queuedPositions.push_back(newPos);
      }
    }

    // Once here, we just want to pop the next position and set as the current
    if (!queuedPositions.empty()) {
      currentPosition = queuedPositions.front();
      queuedPositions.pop_front();
    } else {
      break;
    }
  } while (true);

  // If here, just return player's position
  return this->getEntity()->getPosition();
}

Position Explorer::getNextAutoExploreDestination(Engine& engine) {
  // Start traversing from the Player's location, breadth-first. The first unexplored open tile is returned
  // TODO: factor in locked doors (especially once unlocked)
  // TODO: pick up items on the way
  std::unordered_set<Position> exploredTiles;
  std::deque<Position> queuedPositions;

  Position currentPosition = this->getEntity()->getPosition();
  ExploredGrid& grid = this->getCurrentMap();
  do {
    // If current position is a locked door that we have explored, ignore
    Entity* e = engine.getCurrentMap().getBlockingEntity(currentPosition);
    if (!(e && grid.autoExploreIgnored.contains(e->getID()))) {
      // Get the neighbors of the current position
      for (int i = 0; i < static_cast<int>(Direction::STATIONARY); i++) {
        Direction dir = static_cast<Direction>(i);
        Position newPos = currentPosition + dir;
        if (exploredTiles.contains(newPos)) {
          continue;
        }
        int loc = GameMap::getLocInTileVector(newPos, grid.width);
        Tile t = engine.getCurrentMap().getTileAt(newPos);
        // If unexplored, return immediately
        if (t.isWalkable && !grid.explored[loc]) {
          this->setAutoExploreDesitionation(newPos);
          return newPos;
        }

        // TODO: if wall and unexplored, map to nearby tile (the one we just came from)
        if (t.isWall && !grid.explored[loc]) {
          this->setAutoExploreDesitionation(currentPosition);
          return currentPosition;
        }

        // If we are here, it's been explored. Ignore walls from exploring
        if (t.isWalkable) {
          // Tile is walkable, if we haven't queued it, do so
          if (!exploredTiles.contains(newPos)) {
            exploredTiles.emplace(newPos);
            queuedPositions.push_back(newPos);
          }
        }
      }
    }

    // Once here, we just want to pop the next position and set as the current
    if (!queuedPositions.empty()) {
      currentPosition = queuedPositions.front();
      queuedPositions.pop_front();
    } else {
      break;
    }
  } while (true);

  // If here, there were no unexplored tiles to explore. Return current position as sentinel
  this->resetAutoExploreDestination();
  return this->getEntity()->getPosition();
}

void Explorer::ignoreEntityForAutoExplore(IDGenerator::ID eID) {
  ExploredGrid& grid = this->getCurrentMap();
  grid.autoExploreIgnored.insert(eID);
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
