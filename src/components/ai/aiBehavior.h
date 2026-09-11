#pragma once

#include <memory>

#include "actions/action.h"

class Engine;

class AIBehavior {
 public:
  virtual ~AIBehavior() = default;
  virtual std::unique_ptr<Action> getAction(
      Engine& engine, Entity* entity);  // TODO: Include Pathfinding Callback (ITCODPathCallback)
};
