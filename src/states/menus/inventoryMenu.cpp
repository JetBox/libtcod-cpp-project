#include "inventoryMenu.h"

#include <vector>

#include "actions/dropAction.h"
#include "actions/equipAction.h"
#include "actions/errorAction.h"
#include "common/entity.h"
#include "components/inventory.h"
#include "core/engine.h"

InventoryMenu::InventoryMenu(
    std::string title, std::vector<Entity*> itemList, ActionFn onChoose, std::function<bool(Entity*)> filter) {
  this->title = std::move(title);
  this->footer = "up/down/8/2 move | Enter select | Esc back";
  this->onChoose = onChoose;
  this->filter = filter;
  this->itemList = itemList;
  setSize(40, 30);
}

bool InventoryMenu::onSelect(Engine& engine, int itemIndex) {
  if (itemIndex < 0 || itemIndex >= static_cast<int>(itemOrder.size())) {
    return true;
  }
  Entity* chosen = itemOrder[itemIndex];
  return onChoose(engine, chosen);
}

std::function<bool(Engine&, Entity*)> inspectMenuOnChoose = [](Engine& engine, Entity* entity) {
  engine.getMessageLog().add(Message("You inspect the " + entity->getName() + "."));
  return true;
};

std::function<bool(Engine&, Entity*)> wearMenuOnChoose = [](Engine& engine, Entity* entity) {
  if (engine.getPlayer()->hasComponent<Inventory>()) {
    int itemIndex = engine.getPlayer()->getComponent<Inventory>().findIndexOfItem(entity);
    if (itemIndex == -1) {
      engine.queueAction(std::make_unique<ErrorAction>("ERROR: Item not found to equip."));
      return false;
    }
    engine.queueAction(std::make_unique<EquipAction>(itemIndex));
  } else {
    engine.queueAction(std::make_unique<ErrorAction>("UNEXPECTED ERROR: Entity has no inventory to equip from."));
  }
  return false;  // exit menu
};

std::function<bool(Engine&, Entity*)> dropMenuOnChoose = [](Engine& e, Entity* entity) {
  if (e.getPlayer()->hasComponent<Inventory>()) {
    int itemIndex = e.getPlayer()->getComponent<Inventory>().findIndexOfItem(entity);
    if (itemIndex == -1) {
      e.queueAction(std::make_unique<ErrorAction>("ERROR: Item not found to drop."));
      return false;
    }
    e.queueAction(std::make_unique<DropAction>(itemIndex));
  } else {
    e.queueAction(std::make_unique<ErrorAction>("UNEXPECTED ERROR: Player has no inventory to drop from."));
  }
  return false;  // exit menu
};
