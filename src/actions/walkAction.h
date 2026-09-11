#pragma once

#include "action.h"

class WalkAction : public Action {
 public:
  WalkAction(Direction dir) : dir(dir) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
};
