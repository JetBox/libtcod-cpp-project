#pragma once

#include "states/gameState.h"

class AlertBox : public GameState {
 public:
  AlertBox(std::string message) : message(message) {}

  bool handleEvent(Engine& engine, SDL_Event* event) override;
  void render(Engine& engine, tcod::Console* console) override;
  bool renderStateBelow() const override { return true; }

 private:
  std::string message;
};
