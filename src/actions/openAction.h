#pragma once

#include "action.h"

// Open Actions also can close things, if the entity they are pointing to is already open
class OpenAction : public Action {
 public:
  OpenAction(Direction dir) : dir(dir) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
};

extern std::function<bool(Engine&, Entity*, Direction dir)> openActionOnSelect;
