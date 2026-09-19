#pragma once

#include "gameState.h"

class TileSelectionState : public GameState {
 public:
  TileSelectionState(bool inspectMode, Position startingPosition, std::function<bool(Engine&, Position)> onEnter)
      : inspectMode(inspectMode), currentPosition(startingPosition), onEnter(onEnter) {}

  void update(Engine& engine) override;
  bool handleEvent(Engine& engine, SDL_Event* event) override;
  void render(Engine& engine, tcod::Console* console) override;
  bool renderStateBelow() const override { return true; }

 protected:
  bool inspectMode = false;
  static constexpr int MAX_ENTITIES_TO_VIEW = 5;
  static constexpr int FLASH_FREQUENCY = 60;
  int flashCounter = 0;
  bool flash = false;
  Position currentPosition;
  std::function<bool(Engine&, Position)> onEnter;

  Position getDrawPosition(Engine& engine);
};

extern std::function<bool(Engine& engine, Position)> lookOnEnter;
