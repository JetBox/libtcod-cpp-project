#include "directionSelector.h"

#include <SDL3/SDL_keycode.h>

#include <libtcod/console_printing.hpp>

#include "actions/openAction.h"
#include "common/colors.h"
#include "common/position.h"
#include "core/engine.h"
#include "core/windowFrame.h"

bool DirectionSelector::handleEvent(Engine& engine, SDL_Event* event) {
  if (event->type != SDL_EVENT_KEY_DOWN) {
    return true;
  }

  const SDL_Keycode key = event->key.key;

  switch (key) {
    case SDLK_ESCAPE:
      return false;
    case SDLK_LEFT:
    case SDLK_KP_4:
      this->onSelect(engine, engine.getPlayer(), Direction::WEST);
      return false;
    case SDLK_RIGHT:
    case SDLK_KP_6:
      this->onSelect(engine, engine.getPlayer(), Direction::EAST);
      return false;
    case SDLK_UP:
    case SDLK_KP_8:
      this->onSelect(engine, engine.getPlayer(), Direction::NORTH);
      return false;
    case SDLK_DOWN:
    case SDLK_KP_2:
      this->onSelect(engine, engine.getPlayer(), Direction::SOUTH);
      return false;
    case SDLK_KP_7:
      engine.queueAction(std::make_unique<OpenAction>(Direction::NORTHWEST));
      return false;
    case SDLK_KP_9:
      this->onSelect(engine, engine.getPlayer(), Direction::NORTHEAST);
      return false;
    case SDLK_KP_1:
      this->onSelect(engine, engine.getPlayer(), Direction::SOUTHWEST);
      return false;
    case SDLK_KP_3:
      this->onSelect(engine, engine.getPlayer(), Direction::SOUTHEAST);
      return false;
  }
  return true;
}

void DirectionSelector::render(Engine& /*engine*/, tcod::Console* console) {
  int w = 40;
  int h = 3;
  int x = (VIEW_WIDTH - w) / 2;
  int y = 2;

  WindowFrame frame = WindowFrame(x, y, w, h);
  frame.setTitle(this->title);
  auto [cx, cy, cw, ch] = frame.draw(*console);
  tcod::print_rect(*console, {cx, cy}, "Please select a direction.", WHITE, std::nullopt);
}
