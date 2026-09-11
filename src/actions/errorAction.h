#pragma once

#include "action.h"

class ErrorAction : public Action {
 public:
  ErrorAction(std::string errorMessage) : errorMessage(errorMessage) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

 private:
  std::string errorMessage;
};
