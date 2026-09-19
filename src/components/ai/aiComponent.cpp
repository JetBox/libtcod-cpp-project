#include "aiComponent.h"

#include <libtcod/bresenham.hpp>
#include <memory>

#include "actions/walkAction.h"
#include "common/position.h"
#include "core/engine.h"

std::unique_ptr<Action> AIComponent::getAction(Engine& engine) {
  constexpr int MAX_ENEMY_VISION = 8;
  // Pathfind to the player (only if enemy can see the player)
  Position pp = engine.getPlayer()->getPosition();
  if (pp.chebyshevDistance(this->getEntity()->getPosition()) > MAX_ENEMY_VISION) {
    // Too far away for enemy to see, do nothing
    return std::make_unique<WalkAction>(Direction::STATIONARY);
  }

  bool canPathToPlayer = true;
  tcod::BresenhamLine line =
      tcod::BresenhamLine(this->getEntity()->getPosition().getAsArray(), pp.getAsArray()).without_start();
  for (auto&& [x, y] : line) {
    bool transparent = engine.getCurrentMap().getTileAt({x, y}).isTransparent &&
                       !engine.getCurrentMap().hasVisionBlockingEntity({x, y});
    if (!transparent) {
      canPathToPlayer = false;
      break;
    }
  }

  if (canPathToPlayer) {
    Position startingPosition = this->getEntity()->getPosition();
    Position playerPosition = engine.getPlayer()->getPosition();
    Position nextStep = engine.getCurrentMap().getNextStep(startingPosition, playerPosition, this->getEntity());
    Direction dir = getDirectionToPosition(startingPosition, nextStep);

    return std::make_unique<WalkAction>(dir);
  }

  return std::make_unique<WalkAction>(Direction::STATIONARY);
}
