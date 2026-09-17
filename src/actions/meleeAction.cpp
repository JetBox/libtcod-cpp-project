#include "meleeAction.h"

#include "action.h"
#include "components/combat.h"
#include "core/engine.h"

ActionResult MeleeAction::perform(Engine& engine, Entity* entity) {
  Position endPosition = entity->getPosition() + this->dir;
  Entity* target = engine.getCurrentMap().getBlockingEntity(endPosition);
  if (!target || !target->hasComponent<Fighter>() || !entity->hasComponent<Fighter>()) {
    return ActionResult(false);
  } else {
    int attackPower = getMeleeAttackPower(entity);

    // TODO: determine dodges

    // TODO: determine armor

    // Defender takes damage
    target->getComponent<Fighter>().takeDamage(attackPower);

    // No way to damage for now, so just say success
    return ActionResult(true, target->getName() + " took damage.");
  }
}

int getMeleeAttackPower(Entity* entity) {
  Fighter attacker = entity->getComponent<Fighter>();
  int basePower = attacker.getBasePower();

  // TODO: get power upgrades from other sources

  return basePower;
}
