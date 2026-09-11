#pragma once

#include "action.h"

class MeleeAction : public Action {
 public:
  MeleeAction(Direction dir, int damage) : dir(dir), damage(damage) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
  int damage;
};
