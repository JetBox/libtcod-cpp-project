#include "equipAction.h"

#include "action.h"
#include "common/entity.h"
#include "components/inventory.h"
#include "components/item.h"
#include "core/engine.h"

ActionResult EquipAction::perform(Engine& /*engine*/, Entity* entity) {
  if (!entity->hasComponent<Inventory>() || !entity->hasComponent<Body>()) {
    return ActionResult(false, "ERROR: Missing components for EquipAction.");
  }
  Inventory& inv = entity->getComponent<Inventory>();
  Entity* itemRef = inv.getItem(this->itemIndex);

  // Get the BodyPart for this type of item
  if (!itemRef->hasComponent<Item>() || !itemRef->hasComponent<Equippable>()) {
    return ActionResult(false, "ERROR: Tried to equip a non-equippable item.");
  }
  BodyPartKind slot = itemRef->getComponent<Equippable>().getSlot();

  // TODO: check the item to make sure it fits the criteria to be equipped
  // TODO: example: two-handed weapons, class/size constraints

  // Get the Entity that's already equipped to the same spot
  Body& body = entity->getComponent<Body>();
  auto part = body.find(slot);
  if (!part) {
    return ActionResult(false, "You can't equip this item.");
  } else if (!part->canEquip) {
    return ActionResult(false, "Because of your " + part->name + ", you aren't capable of equipping this item.");
  }
  std::string actionResultMessage;
  auto equippedItemRef = inv.findEquippedItem(slot);

  // If there's something already equipped, we need to unequip it first
  if (equippedItemRef) {
    if (!equippedItemRef->hasComponent<Equippable>() || !equippedItemRef->hasComponent<Item>()) {
      return ActionResult(false, "ERROR: Already equipped item has improper components.");
    }
    auto equippedItemEquippable = equippedItemRef->getComponent<Equippable>();
    auto equippedItemItem = equippedItemRef->getComponent<Item>();
    if (equippedItemEquippable.getIsCursed()) {
      return ActionResult(false, "You can't unequip the " + equippedItemItem.getName() + "!");
    }

    // Remove the item from the body
    equippedItemEquippable.setIsEquipped(false);
    actionResultMessage = "You swapped your " + equippedItemRef->getComponent<Item>().getName() + " with your " +
                          itemRef->getComponent<Item>().getName() + ".";
  } else {
    actionResultMessage = "You equipped your " + itemRef->getComponent<Item>().getName();
  }

  // set equipped
  itemRef->getComponent<Equippable>().setIsEquipped(true);

  // TODO: allow the Message to have multiple messages, for displaying multiple lines of happenings
  return ActionResult(true, actionResultMessage);
}
