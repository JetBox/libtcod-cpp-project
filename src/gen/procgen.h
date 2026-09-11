#pragma once

#include <libtcod/mersenne.hpp>

#include "common/idGenerator.h"
#include "core/gameMap.h"
#include "core/gameWorld.h"
#include "rectangularRoom.h"

// Map generation
void generateDungeon(
    GameWorld& world,
    IDGenerator::ID mapID,
    int maxRooms,
    int minSize,
    int maxSize,
    int maxMonstersPerRoom,
    int maxItemsPerRoom);

// Room Generation
void carveRoom(RectangularRoom room, GameMap& map);
void carveTunnel(GameMap& map, Position start, Position stop, bool withoutStart = false);
void carveLTunnel(GameMap& map, TCODRandom* rng, Position start, Position end);

bool isWallsOnBothSides(GameMap& map, Position p);

// Monster & Item Placement
void placeEntities(
    RectangularRoom room, GameWorld& world, IDGenerator::ID mapID, TCODRandom* rng, int maxMonsters, int maxItems);

std::unique_ptr<Entity> createChest(Position p, TCODRandom* rng);
std::unique_ptr<Entity> createKey(Position p);
