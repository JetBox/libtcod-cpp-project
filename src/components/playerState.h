#pragma once

#include "components/component.h"

enum class PlayerState { DEAD, PLAYING, AUTO_EXPLORE };

class PlayerStateComponent : public BaseComponent {
 public:
  PlayerStateComponent(PlayerState initialState) : currentState(initialState) {}

  PlayerState getCurrentState() { return this->currentState; }
  void setState(PlayerState newState) { this->currentState = newState; }

  // Game Processing

 private:
  PlayerState currentState;
};
