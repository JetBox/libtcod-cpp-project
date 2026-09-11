#pragma once

#include <SDL3/SDL.h>

#include <libtcod.hpp>

#include "SDL3/SDL_events.h"
#include "actions/action.h"

class Engine;

class GameState {
 public:
  virtual ~GameState() = default;

  virtual void update(Engine& /*engine*/) {};

  virtual bool handleEvent(Engine& engine, SDL_Event* event) = 0;

  virtual void render(Engine& engine, tcod::Console* console) = 0;

  virtual bool renderStateBelow() const { return false; }
};
