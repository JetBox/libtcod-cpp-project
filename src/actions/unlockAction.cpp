#include "unlockAction.h"

#include "action.h"
#include "components/inventory.h"
#include "components/item.h"
#include "components/locked.h"
#include "core/engine.h"

ActionResult UnlockAction::perform(Engine& engine, Entity* entity) {
  // Get Target
  // Get entity at location of etntiy + dir
  Position endLocation = entity->getPosition() + this->dir;
  std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(endLocation);
  Entity* target = nullptr;
  for (auto e : entities) {
    if (e->hasComponent<Locked>() && e->getComponent<Locked>().getIsLocked()) {
      target = e;
      break;
    }
  }
  if (!target) {
    return ActionResult(false, "There's nothing to unlock.");
  }
  Locked& locked = target->getComponent<Locked>();

  // Get keys
  if (!entity->hasComponent<Inventory>()) {
    return ActionResult(false, "ERROR: Need an inventory to unlock.");
  }
  Inventory& inv = entity->getComponent<Inventory>();
  int itemIdx = inv.findItemByName("key");
  if (itemIdx == -1) {
    return ActionResult(false, "You need a key!");
  }
  Entity* keyItem = inv.getItem(itemIdx);

  // Decrement keys
  bool success = keyItem->getComponent<Item>().remove(1);
  if (!success) {
    return ActionResult(false, "You don't have enough keys!");
  }

  // Remove key if at 0 count
  if (keyItem->getComponent<Item>().getStackSize() == 0) {
    // Remove from the inventory
    inv.removeItem(itemIdx);
  }

  // Unlock the door
  locked.unlock();

  return ActionResult(true, "You unlocked the " + target->getName() + ".");
}

std::function<bool(Engine&, Entity*, Direction dir)> unlockActionOnSelect =
    [](Engine& engine, Entity* /* entity */, Direction dir) {
      engine.queueAction(std::make_unique<UnlockAction>(dir));
      return false;
    };
