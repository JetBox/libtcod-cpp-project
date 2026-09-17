#include "aiComponent.h"

#include <memory>

#include "actions/walkAction.h"
#include "common/position.h"
#include "core/engine.h"

std::unique_ptr<Action> AIComponent::getAction(Engine& engine) {
  // Pathfind to the player (only if enemy can see the player)
  Position startingPosition = this->getEntity()->getPosition();
  Position playerPosition = engine.getPlayer()->getPosition();
  Position nextStep = engine.getCurrentMap().getNextStep(startingPosition, playerPosition);
  Direction dir = getDirectionToPosition(startingPosition, nextStep);

  return std::make_unique<WalkAction>(dir);
}
