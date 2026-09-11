#pragma once

#include <memory>

#include "actions/action.h"
#include "components/component.h"

class Engine;

class AIComponent : public BaseComponent {
 public:
  virtual ~AIComponent() = default;
  virtual std::unique_ptr<Action> getAction(Engine& engine);

 private:
};
