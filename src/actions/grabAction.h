#pragma once

#include "action.h"

class GrabAction : public Action {
 public:
  GrabAction() = default;

  ActionResult perform(Engine& engine, Entity* entity) override;
};
