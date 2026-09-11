#pragma once

#include "action.h"

// Kicks can bust open locked doors
// TODO: Implement kicking enemies
class KickAction : public Action {
 public:
  KickAction(Direction dir) : dir(dir) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  Direction dir;
};

extern std::function<bool(Engine&, Entity*, Direction dir)> kickActionOnSelect;
