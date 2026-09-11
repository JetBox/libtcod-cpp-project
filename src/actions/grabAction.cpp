#include "grabAction.h"

#include <memory>
#include <string>

#include "action.h"
#include "actions/massGrabAction.h"
#include "components/inventory.h"
#include "components/item.h"
#include "core/engine.h"
#include "states/menus/alertBox.h"
#include "states/menus/itemSelectMenu.h"

ActionResult GrabAction::perform(Engine& engine, Entity* entity) {
  if (!entity->hasComponent<Inventory>()) {
    return ActionResult(false, "You can't grab anything!");
  }

  // Get entities on the floor here
  GameMap& map = engine.getCurrentMap();
  std::vector<Entity*> eList = map.getEntitiesAt(entity->getPosition(), entity);

  // Filter out non-Items
  std::vector<Entity*> iList;
  for (auto e : eList) {
    if (e->hasComponent<Item>()) {
      iList.push_back(e);
    }
  }

  if (iList.empty()) {
    return ActionResult(false, "There is nothing to grab.");
  }

  Inventory& inventory = entity->getComponent<Inventory>();
  if (inventory.isFull()) {
    return ActionResult(false, "Your inventory is full.");
  }

  if (iList.size() > 1) {
    engine.pushState(
        std::make_unique<ItemSelectMenu>("Grab Items", iList, [](Engine& engine, std::vector<Entity*> chosen) {
          Inventory& inv = engine.getPlayer()->getComponent<Inventory>();
          int slotsAvailable = inv.getMaxItems() - static_cast<int>(inv.getItems().size());
          if (static_cast<int>(chosen.size()) > slotsAvailable) {
            // Alert Box
            std::string plural = slotsAvailable == 1 ? "." : "s.";
            engine.pushState(
                std::make_unique<AlertBox>(
                    "You can only carry " + std::to_string(slotsAvailable) + " more item" + plural));
            return true;
          }
          // Queue massGrabAction
          engine.queueAction(std::make_unique<MassGrabAction>(chosen));
          return false;
        }));
    return ActionResult(false);
  }

  std::unique_ptr<Entity> item = map.extractEntity(iList[0]);
  std::string name = item->getName();
  ItemAddResult reject = inventory.addItem(std::move(item));

  if (reject.result) {
    // We couldn't move the item, fail
    GameWorld& world = engine.getGameWorld();
    world.addEntityToMap(std::move(reject.result));
    return ActionResult(false, reject.message);
  }

  return ActionResult(true, "You picked up the " + name + ".");
}
