#include "aiComponent.h"

#include <memory>

#include "actions/walkAction.h"

std::unique_ptr<Action> AIComponent::getAction(Engine& /* engine */) {
  // First: test the structure works. All AI will do is walk to the north repeatedly.
  return std::make_unique<WalkAction>(Direction::NORTH);
}
