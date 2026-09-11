#include "gameWorld.h"

#include "common/idGenerator.h"
#include "core/gameMap.h"

IDGenerator::ID GameWorld::createMap(int width, int height) {
  IDGenerator::ID newID = this->gameMapIDGenerator.next();
  this->maps.try_emplace(newID, GameMap(newID, width, height));
  return newID;
}

GameMap& GameWorld::getMap(IDGenerator::ID mapID) { return this->maps.at(mapID); }

void GameWorld::addEntityToMap(std::unique_ptr<Entity> entity, IDGenerator::ID mapID) {
  // Assume a mapID of 0 == current
  if (mapID == 0) {
    mapID = this->activeMap;
  }

  // Assign an ID if it doesn't already exist
  if (entity->getID() == 0) {
    entity->setID(this->entityIDGenerator.next());
  }
  GameMap& map = this->maps.at(mapID);
  map.addEntity(std::move(entity));
}
