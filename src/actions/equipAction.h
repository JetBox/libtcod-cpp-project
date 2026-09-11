#pragma once

#include "action.h"

class EquipAction : public Action {
 public:
  EquipAction(int itemIndex) : itemIndex(itemIndex) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  int itemIndex;
};
