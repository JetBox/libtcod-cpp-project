#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#include <deque>
#include <libtcod.hpp>
#include <memory>

#include "common/entity.h"
#include "common/idGenerator.h"
#include "gameMap.h"
#include "gameWorld.h"
#include "message.h"
#include "states/gameState.h"

static constexpr auto SCREEN_WIDTH = 100;
static constexpr auto SCREEN_HEIGHT = 64;

// Dungeon gen
static constexpr auto MAP_WIDTH = 150;
static constexpr auto MAP_HEIGHT = 80;
static constexpr auto MAX_ROOMS = 30;
static constexpr auto MIN_ROOM_SIZE = 6;
static constexpr auto MAX_ROOM_SIZE = 10;
static constexpr auto MAX_MONSTERS_PER_ROOM = 2;
static constexpr auto MAX_ITEMS_PER_ROOM = 3;

// View
static constexpr auto VIEW_X = 0;
static constexpr auto VIEW_Y = 0;
static constexpr auto VIEW_WIDTH = 80;
static constexpr auto VIEW_HEIGHT = 54;

// Game Map View
static constexpr auto MAP_VIEW_X = VIEW_X + 1;
static constexpr auto MAP_VIEW_Y = VIEW_Y + 1;
static constexpr auto MAP_VIEW_WIDTH = VIEW_WIDTH - 2;
static constexpr auto MAP_VIEW_HEIGHT = VIEW_HEIGHT - 2;

// Log Box
static constexpr auto TEXT_WIDTH = VIEW_WIDTH;
static constexpr auto TEXT_HEIGHT = 10;
static constexpr auto TEXT_X = 0;
static constexpr auto TEXT_Y = 54;

// UI
static constexpr auto UI_WIDTH = SCREEN_WIDTH - VIEW_WIDTH;  // Should be 20
static constexpr auto UI_HEIGHT = SCREEN_HEIGHT;
static constexpr auto UI_X = VIEW_WIDTH;

class Engine {
 public:
  Engine();

  void update();
  SDL_AppResult handleEvent(SDL_Event* event);

  void render(tcod::Console* console, tcod::Context* context);
  void setPlayer(std::unique_ptr<Entity> e);
  Entity* getPlayer() { return this->player; }
  GameWorld& getGameWorld() { return this->gameWorld; }
  GameMap& getCurrentMap() { return this->gameWorld.getActiveMap(); }
  IDGenerator::ID getCurrentMapID() { return this->gameWorld.getActiveMapID(); }
  MessageLog& getMessageLog() { return this->messageLog; }

  Position getCamera() const { return this->camera; }
  void computeCamera();

  void pushState(std::unique_ptr<GameState> state);
  void popState();
  GameState* topState();

  std::deque<std::unique_ptr<Action>>& getQueuedActions() { return this->queuedActions; }

  void queueAction(std::unique_ptr<Action> act);

  TCODRandom* getRNG() { return this->rng; }

  void renderUI(tcod::Console* console);

 private:
  Entity* player;
  GameWorld gameWorld;
  MessageLog messageLog;
  TCODRandom* rng;

  Position camera;

  std::vector<std::unique_ptr<GameState>> states;

  std::deque<std::unique_ptr<Action>> queuedActions;
};
