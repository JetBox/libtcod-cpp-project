#include "kickAction.h"

#include "action.h"
#include "components/combat.h"
#include "components/locked.h"
#include "components/openable.h"
#include "components/vitals.h"
#include "core/engine.h"

ActionResult KickAction::perform(Engine& engine, Entity* entity) {
  // Get entity at location of entity + dir
  Position endLocation = entity->getPosition() + this->dir;
  std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(endLocation);
  Entity* target = nullptr;
  for (auto e : entities) {
    if (e->hasComponent<Openable>() && e->getComponent<Openable>().isClosed()) {
      target = e;
      break;
    }
  }

  // Make a strength check
  if (!entity->hasComponent<BaseStats>()) {
    return ActionResult(false, "ERROR: Need a BaseStats component to kick.");
  }

  // First, decay fatigue
  if (entity->hasComponent<Vitals>()) {
    Vitals& vitals = entity->getComponent<Vitals>();
    vitals.expendFatigue(vitals.FATIGUE_EASY);
  }

  if (!target) {
    return ActionResult(false, "You kick into the air.");
  }

  BaseStats bs = entity->getComponent<BaseStats>();
  bool success = makeStatCheck(bs.getStrength());

  if (!success) {
    return ActionResult(true, "You kicked at the " + target->getName() + " to no avail.");
  }

  // If the target is locked, unlock it
  if (target->hasComponent<Locked>() && target->getComponent<Locked>().getIsLocked()) {
    target->getComponent<Locked>().unlock();
  }

  // Open/close the target
  target->getComponent<Openable>().toggleOpen(engine, target);
  return ActionResult(true, "You busted open the " + target->getName() + ".");
}

std::function<bool(Engine&, Entity*, Direction dir)> kickActionOnSelect =
    [](Engine& engine, Entity* /* entity */, Direction dir) {
      engine.queueAction(std::make_unique<KickAction>(dir));
      return true;
    };
