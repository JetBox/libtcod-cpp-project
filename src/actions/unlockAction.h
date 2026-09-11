#pragma once

#include "action.h"

class UnlockAction : public Action {
 public:
  UnlockAction(Direction dir) : dir(dir) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
};

extern std::function<bool(Engine&, Entity*, Direction dir)> unlockActionOnSelect;
