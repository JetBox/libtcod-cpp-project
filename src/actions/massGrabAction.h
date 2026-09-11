#pragma once

#include "action.h"

class MassGrabAction : public Action {
 public:
  MassGrabAction(std::vector<Entity*> itemsToGrab) : itemsToGrab(itemsToGrab) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  std::vector<Entity*> itemsToGrab;
};
