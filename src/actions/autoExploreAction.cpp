#include "autoExploreAction.h"

#include "components/explorer.h"
#include "components/locked.h"
#include "components/openable.h"
#include "components/playerState.h"
#include "core/engine.h"

ActionResult AutoExploreAction::perform(Engine& engine, Entity* entity) {
  if (entity != engine.getPlayer()) {
    return ActionResult(false, "Can't auto-explore if not player.");
  }

  // Means we can't auto-explore anymore, return to playing
  if (this->dir == Direction::STATIONARY) {
    entity->getComponent<PlayerStateComponent>().setState(PlayerState::PLAYING);
    return ActionResult(false);
  }
  Position nextPos = entity->getPosition() + this->dir;
  Explorer& explorer = entity->getComponent<Explorer>();
  GameMap& map = engine.getCurrentMap();
  if (map.inBounds(nextPos) && map.getTileAt(nextPos).isWalkable) {
    // check if we ran into a door or other stationary thing
    Entity* e = map.getBlockingEntity(nextPos);

    if (e->hasComponent<Openable>() && e->hasComponent<Locked>() && e->getComponent<Locked>().getIsLocked()) {
      // This is a locked door. Ignore this for auto-explore
      explorer.ignoreEntityForAutoExplore(e->getID());
      return ActionResult(false);
    }
  }
}
