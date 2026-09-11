#include "openAction.h"

#include "action.h"
#include "components/locked.h"
#include "components/openable.h"
#include "core/engine.h"

ActionResult OpenAction::perform(Engine& engine, Entity* entity) {
  // Get entity at location of entity + dir
  Position endLocation = entity->getPosition() + this->dir;
  std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(endLocation);
  Entity* target = nullptr;
  for (auto e : entities) {
    if (e->hasComponent<Openable>()) {
      target = e;
      break;
    }
  }

  if (!target) {
    return ActionResult(false, "There's nothing there to open.");
  }

  // Make sure target isn't locked
  if (target->hasComponent<Locked>() && target->getComponent<Locked>().getIsLocked()) {
    return ActionResult(true, "The " + target->getName() + " is locked.");
  }
  // TODO: Option to allow auto-unlocking with keys.

  // Open/close the target
  target->getComponent<Openable>().toggleOpen(engine, target);
  return ActionResult(true);
}

std::function<bool(Engine&, Entity*, Direction dir)> openActionOnSelect =
    [](Engine& engine, Entity* /* entity */, Direction dir) {
      engine.queueAction(std::make_unique<OpenAction>(dir));
      return false;
    };
