#pragma once

#include <string>

#include "core/engine.h"
#include "states/gameState.h"

class DirectionSelector : public GameState {
 public:
  using ActionFn = std::function<bool(Engine&, Entity*, Direction dir)>;

  DirectionSelector(ActionFn onSelect, std::string title = "") : onSelect(onSelect), title(title) {}

  bool handleEvent(Engine& engine, SDL_Event* event) override;
  void render(Engine& engine, tcod::Console* console) override;
  bool renderStateBelow() const override { return true; }

 private:
  ActionFn onSelect;
  std::string title;
};
