#include "meleeAction.h"

#include "action.h"
#include "core/engine.h"

ActionResult MeleeAction::perform(Engine& engine, Entity* entity) {
  Position endPosition = entity->getPosition() + this->dir;
  Entity* target = engine.getCurrentMap().getBlockingEntity(endPosition);
  if (!target) {
    // We tried to melee with no one, so do nothing
    return ActionResult(false);
  } else {
    // No way to damage for now, so just say success
    return ActionResult(true, "Do some damage!");
  }
}
