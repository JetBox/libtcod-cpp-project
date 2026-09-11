#include "dropAction.h"

#include <memory>

#include "action.h"
#include "components/inventory.h"
#include "components/item.h"
#include "core/engine.h"

ActionResult DropAction::perform(Engine& engine, Entity* entity) {
  if (!entity->hasComponent<Inventory>()) {
    return ActionResult(false, "ERROR: Missing Inventory Component for Drop Action!");
  }
  std::string droppedMessage = "You ";
  Inventory& inv = entity->getComponent<Inventory>();
  Entity* itemRef = inv.getItem(this->itemIndex);
  if (!itemRef) {
    return ActionResult(false, "ERROR: Item not found for Drop Action!");
  } else if (!itemRef->hasComponent<Item>()) {
    return ActionResult(false, "ERROR: Tried to drop something that wasn't an item.");
  }
  Item& item = itemRef->getComponent<Item>();

  // Check if the item is equipped, unequip if so
  if (itemRef->hasComponent<Equippable>()) {
    Equippable& eq = itemRef->getComponent<Equippable>();
    if (eq.getIsEquipped()) {
      if (eq.getIsCursed()) {
        return ActionResult(false, "The " + item.getName() + "'s curse prevents you from unequipping it!");
      }
      eq.setIsEquipped(false);
      droppedMessage += "unequiped and ";
    }
  }

  // set the Item's coords to the entities
  itemRef->setPosition(entity->getPosition());

  // Remove the item from inventory, place in GameMap entity list
  droppedMessage += "dropped the " + item.getName() + ".";
  std::unique_ptr<Entity> droppedItem = inv.removeItem(this->itemIndex);
  GameWorld& world = engine.getGameWorld();
  world.addEntityToMap(std::move(droppedItem));

  return ActionResult(true, droppedMessage);
}
