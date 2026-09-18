#pragma once

#include "action.h"

class AutoExploreAction : public Action {
 public:
  AutoExploreAction(Direction dir) : dir(dir) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
};
