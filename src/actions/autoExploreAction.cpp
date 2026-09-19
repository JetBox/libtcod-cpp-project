#include "autoExploreAction.h"

#include "action.h"
#include "actions/openAction.h"
#include "components/explorer.h"
#include "components/locked.h"
#include "components/openable.h"
#include "components/playerState.h"
#include "core/engine.h"

ActionResult AutoExploreAction::perform(Engine& engine, Entity* entity) {
  if (entity != engine.getPlayer()) {
    return ActionResult(false, "Can't auto-explore if not player.");
  }

  Explorer& explorer = entity->getComponent<Explorer>();
  // Means we can't auto-explore anymore, return to playing
  if (this->dir == Direction::STATIONARY) {
    explorer.resetAutoExploreDestination();
    entity->getComponent<PlayerStateComponent>().setState(PlayerState::PLAYING);
    return ActionResult(false);
  }
  Position nextPos = entity->getPosition() + this->dir;
  GameMap& map = engine.getCurrentMap();
  if (map.inBounds(nextPos) && map.getTileAt(nextPos).isWalkable) {
    // check if we ran into a door or other stationary thing
    Entity* e = map.getBlockingEntity(nextPos);

    if (e && e->hasComponent<Openable>() && e->hasComponent<Locked>() && e->getComponent<Locked>().getIsLocked()) {
      // This is a locked door. Ignore this for auto-explore
      explorer.ignoreEntityForAutoExplore(e->getID());
      explorer.resetAutoExploreDestination();
      return ActionResult(false);
    }

    // Otherwise, open doors
    if (e && e->hasComponent<Openable>()) {
      return ActionResult(std::make_unique<OpenAction>(this->dir));
    }

    // Otherwise, walk
    entity->move(this->dir);
    explorer.setAutoExploreDesitionation({-1, -1});
    return ActionResult(true);
  }
  // Somehow can't walk here
  explorer.resetAutoExploreDestination();
  return ActionResult(false, "Auto-explore error.");
}
