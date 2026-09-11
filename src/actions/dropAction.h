#pragma once

#include "action.h"

class DropAction : public Action {
 public:
  DropAction(int itemIndex) : itemIndex(itemIndex) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  int itemIndex;
};
