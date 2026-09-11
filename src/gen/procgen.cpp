#include "procgen.h"

#include <libtcod/bresenham.hpp>
#include <memory>
#include <utility>

#include "common/colors.h"
#include "common/entity.h"
#include "common/glyphs.h"
#include "common/idGenerator.h"
#include "common/position.h"
#include "components/explorable.h"
#include "components/item.h"
#include "components/locked.h"
#include "components/openable.h"
#include "factories/item_factory.h"
#include "rectangularRoom.h"

void generateDungeon(
    GameWorld& world,
    IDGenerator::ID mapID,
    int maxRooms,
    int minSize,
    int maxSize,
    int maxMonstersPerRoom,
    int maxItemsPerRoom) {
  TCODRandom* rng = TCODRandom::getInstance();
  GameMap& dungeon = world.getMap(mapID);

  std::vector<RectangularRoom> roomList;
  RectangularRoom lastRoom;

  for (int i = 0; i < maxRooms; i++) {
    int roomWidth = rng->getInt(minSize, maxSize);
    int roomHeight = rng->getInt(minSize, maxSize);

    int sx = rng->getInt(0, dungeon.getWidth() - roomWidth - 1);
    int sy = rng->getInt(0, dungeon.getHeight() - roomHeight - 1);

    auto newRoom = RectangularRoom(Position(sx, sy), roomWidth, roomHeight);

    // Check if this room intersects with any other room we've made
    bool breakAndContinue = false;
    for (auto room : roomList) {
      if (newRoom.intersects(room)) {
        breakAndContinue = true;
        break;
      }
    }
    if (breakAndContinue) {
      continue;
    }

    // Carve the room
    carveRoom(newRoom, dungeon);

    // Carve a tunnel between this room and the previous
    if (i != 0) {
      carveLTunnel(dungeon, rng, newRoom.center(), lastRoom.center());
    } else {
      dungeon.setOrigin(newRoom.center());
    }

    // Spawn entities in the room
    placeEntities(newRoom, world, mapID, rng, maxMonstersPerRoom, maxItemsPerRoom);

    roomList.push_back(newRoom);
    lastRoom = newRoom;
  }

  // Place doors in rooms
  for (auto room : roomList) {
    for (int x = room.x1() - 1; x < room.x2() + 1; x++) {
      for (int y = room.y1() - 1; y < room.y2() + 1; y++) {
        // only check outer edges
        if (x == room.x1() - 1 || x == room.x2() || y == room.y1() - 1 || y == room.y2()) {
          // If carved, place a door 25% of the time
          if (dungeon.getTileAt(x, y).isWalkable && isWallsOnBothSides(dungeon, {x, y}) &&
              !dungeon.getBlockingEntity({x, y}) && rng->getInt(1, 4) == 1) {
            std::unique_ptr<Entity> newDoor = std::make_unique<Entity>(
                Position(x, y), DOOR_CHAR, WALL_LIGHT, "door", RenderOrder::Interactable, true, true);
            newDoor->addComponent<Explorable>(WALL_DARK, true);
            newDoor->addComponent<Openable>(DOOR_CHAR, DOOR_OPEN_CHAR, true, true);

            // Make doors locked 25% of the time
            if (rng->getInt(1, 4) == 1) {
              newDoor->addComponent<Locked>();
            }

            world.addEntityToMap(std::move(newDoor), mapID);
          }
        }
      }
    }
  }
}

bool isWallsOnBothSides(GameMap& map, Position p) {
  return (map.getTileAt(p + Direction::NORTH).isWall && map.getTileAt(p + Direction::SOUTH).isWall) ||
         (map.getTileAt(p + Direction::WEST).isWall && map.getTileAt(p + Direction::EAST).isWall);
}

void carveRoom(RectangularRoom room, GameMap& map) {
  for (int x = room.x1(); x < room.x2(); x++) {
    for (int y = room.y1(); y < room.y2(); y++) {
      map.carveFloor(Position(x, y));
    }
  }
}

void carveTunnel(GameMap& map, Position start, Position stop, bool withoutStart) {
  if (withoutStart) {
    for (auto&& [x, y] : tcod::BresenhamLine(start.getAsArray(), stop.getAsArray()).without_start()) {
      map.carveFloor(Position(x, y));
    }
  } else {
    for (auto&& [x, y] : tcod::BresenhamLine(start.getAsArray(), stop.getAsArray())) {
      map.carveFloor(Position(x, y));
    }
  }
}

// TODO: use tcod's random
void carveLTunnel(GameMap& map, TCODRandom* rng, Position start, Position end) {
  Position corner;
  if (rng->getInt(0, 1) == 0) {
    corner = Position(end.x, start.y);
  } else {
    corner = Position(start.x, end.y);
  }

  // Carve both tunnels
  carveTunnel(map, start, corner);
  carveTunnel(map, corner, end);
}

void placeEntities(
    RectangularRoom room, GameWorld& world, IDGenerator::ID mapID, TCODRandom* rng, int maxMonsters, int maxItems) {
  int numMonsters = rng->getInt(0, maxMonsters);
  GameMap& map = world.getMap(mapID);

  // Monsters
  for (int i = 0; i < numMonsters; i++) {
    int rx = rng->getInt(room.x1() + 1, room.x2() - 1);
    int ry = rng->getInt(room.y1() + 1, room.y2() - 1);

    // Make sure we haven't already spawned an entity here
    bool canSpawn = true;
    for (const auto& e : map.getEntities()) {
      if (e->getPosition() == Position(rx, ry)) {
        canSpawn = false;
        break;
      }
    }

    if (canSpawn) {
      if (rng->getInt(0, 9) < 8) {
        // Spawn ORC 80% of time
        world.addEntityToMap(spawnOrc(Position(rx, ry)), mapID);
      } else {
        // Spawn TROLL 20% of time
        world.addEntityToMap(spawnTroll(Position(rx, ry)), mapID);
      }
    }
  }

  // Items
  int numItems = rng->getInt(0, maxItems);
  for (int i = 0; i < numItems; i++) {
    int rx = rng->getInt(room.x1() + 1, room.x2() - 1);
    int ry = rng->getInt(room.y1() + 1, room.y2() - 1);

    // Create a chest 25% of the time
    int rngNum = rng->getInt(1, 4);
    if (rngNum == 1) {
      world.addEntityToMap(createChest({rx, ry}, rng), mapID);
    } else {
      // spawn key
      world.addEntityToMap(createKey({rx, ry}), mapID);
    }
  }
}

std::unique_ptr<Entity> createKey(Position p) {
  std::unique_ptr<Entity> newKey =
      std::make_unique<Entity>(p, KEY_GLYPH, ENTITY_WHITE, "key", RenderOrder::Item, false, false);
  newKey->addComponent<Item>("key", "", 99);
  return std::move(newKey);
}

std::unique_ptr<Entity> createChest(Position p, TCODRandom* rng) {
  std::unique_ptr<Entity> newChest =
      std::make_unique<Entity>(p, UNOPENED_CHEST, ENTITY_WHITE, "chest", RenderOrder::Interactable, true, false);

  newChest->addComponent<Explorable>(WALL_DARK);
  newChest->addComponent<Openable>(
      UNOPENED_CHEST, OPENED_CHEST, true, false, false, true, [](Engine& engine, Entity* opener, Entity* openable) {
        // When opening a chest, spawn a random equipment
        engine.getGameWorld().addEntityToMap(getRandomEquipment(engine.getRNG(), openable->getPosition()));
        if (opener == engine.getPlayer()) {
          engine.getMessageLog().add({"You opened the chest and found an item!"});
        }
      });

  // Make chest locked 75% of the time
  if (rng->getInt(1, 4) > 1) {
    newChest->addComponent<Locked>();
  }

  return newChest;
}
