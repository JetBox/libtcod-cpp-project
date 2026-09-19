#include "tileSelectionState.h"

#include <SDL3/SDL_keycode.h>

#include <algorithm>
#include <libtcod/console_printing.hpp>
#include <memory>
#include <string>

#include "actions/castAction.h"
#include "common/colors.h"
#include "common/position.h"
#include "core/engine.h"

void TileSelectionState::update(Engine& /* engine */) {
  this->flashCounter++;
  if (this->flashCounter == TileSelectionState::FLASH_FREQUENCY) {
    this->flash = !this->flash;
    this->flashCounter = 0;
  }
}

bool TileSelectionState::handleEvent(Engine& engine, SDL_Event* event) {
  if (event->type != SDL_EVENT_KEY_DOWN) {
    return true;
  }

  if (event->key.key == SDLK_ESCAPE) {
    return false;
  }

  if (event->key.key == SDLK_KP_ENTER || event->key.key == SDLK_RETURN) {
    return this->onEnter(engine, this->currentPosition);
  }

  Position newPosition = this->currentPosition;
  switch (event->key.key) {
    case SDLK_UP:
    case SDLK_KP_8:
      newPosition += Direction::NORTH;
      break;
    case SDLK_DOWN:
    case SDLK_KP_2:
      newPosition += Direction::SOUTH;
      break;
    case SDLK_RIGHT:
    case SDLK_KP_6:
      newPosition += Direction::EAST;
      break;
    case SDLK_LEFT:
    case SDLK_KP_4:
      newPosition += Direction::WEST;
      break;
    case SDLK_KP_1:
      newPosition += Direction::SOUTHWEST;
      break;
    case SDLK_KP_3:
      newPosition += Direction::SOUTHEAST;
      break;
    case SDLK_KP_7:
      newPosition += Direction::NORTHWEST;
      break;
    case SDLK_KP_9:
      newPosition += Direction::NORTHEAST;
      break;
    default:
      return true;
  }

  if (!engine.getCurrentMap().inBounds(newPosition)) {
    return true;
  }
  Position screen = newPosition - engine.getCamera();
  if (screen.x < 0 || screen.y < 0 || screen.x >= MAP_VIEW_WIDTH || screen.y >= MAP_VIEW_HEIGHT) {
    return true;
  }
  this->currentPosition = newPosition;
  return true;
}

void TileSelectionState::render(Engine& engine, tcod::Console* console) {
  Position drawPosition = this->getDrawPosition(engine);

  // Draw an 'X' where the currentPosition is
  if (!this->flash) {
    tcod::print(*console, drawPosition.getAsArray(), "X", RED, std::nullopt);
  }

  // Get entities at the tile
  // TODO: better text when inspecting (a/an, sorting, etc.)
  if (this->inspectMode) {
    Tile t = engine.getCurrentMap().getTileAt(this->currentPosition);
    std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(this->currentPosition);
    std::string message = "";
    if (entities.empty()) {
      if (t.isWall) {
        message += "a wall";
      }
    }
    int i = 0;
    for (; i < std::min(static_cast<int>(entities.size()), MAX_ENTITIES_TO_VIEW); ++i) {
      Entity* e = entities.at(i);
      message += "a " + e->getName();
      if (i != static_cast<int>(entities.size()) - 1) {
        message += ", ";
      }
    }
    if (i < static_cast<int>(entities.size())) {
      message += " and " + std::to_string(entities.size() - i) + " more.";
    }
    tcod::print(*console, {MAP_VIEW_X, MAP_VIEW_Y + MAP_VIEW_HEIGHT - 1}, message, BLACK, WHITE);
  }
}

Position TileSelectionState::getDrawPosition(Engine& engine) {
  return this->currentPosition - engine.getCamera() + Position{MAP_VIEW_X, MAP_VIEW_Y};
}

std::function<bool(Engine& engine, Position)> lookOnEnter = [](Engine& /* engine */, Position /* p */) { return true; };
