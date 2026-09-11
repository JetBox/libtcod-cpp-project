#pragma once

#include <memory>

#include "gameState.h"

class PlayState : public GameState {
 public:
  void update(Engine& engine) override;
  bool handleEvent(Engine& engine, SDL_Event* event) override;
  void render(Engine& engine, tcod::Console* console) override;
  bool renderStateBelow() const override { return false; }

 private:
  std::unique_ptr<Action> processKeyDown(SDL_KeyboardEvent ev);
  void processEntityTurn(Engine& engine, Entity* entity);
};
