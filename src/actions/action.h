#pragma once

#include <memory>

#include "common/entity.h"
#include "components/speed.h"
#include "core/message.h"

class Engine;
struct ActionResult;

class Action {
 public:
  virtual ActionResult perform(Engine& engine, Entity* entity) = 0;
  virtual ~Action() = default;
};

struct ActionResult {
  std::unique_ptr<Action> alternate = nullptr;
  bool succeeded = false;
  Message message;
  int energyCost = Speed::DEFAULT_ENERGY_COST;

  ActionResult() = default;

  ActionResult(bool succeeded, Message msg = Message(), int cost = Speed::DEFAULT_ENERGY_COST) {
    this->succeeded = succeeded;
    this->message = msg;
    this->energyCost = cost;
  }

  ActionResult(bool succeeded, std::string msg, int cost = Speed::DEFAULT_ENERGY_COST)
      : ActionResult(succeeded, Message(std::move(msg)), cost) {}

  ActionResult(std::unique_ptr<Action> action, Message msg = Message(), int cost = Speed::DEFAULT_ENERGY_COST) {
    this->alternate = std::move(action);
    this->message = msg;
    this->energyCost = cost;
  }

  ActionResult(std::unique_ptr<Action> action, std::string msg, int cost = Speed::DEFAULT_ENERGY_COST)
      : ActionResult(std::move(action), Message(std::move(msg)), cost) {}
};
