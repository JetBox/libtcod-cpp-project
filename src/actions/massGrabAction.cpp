#include "massGrabAction.h"

#include "action.h"
#include "components/inventory.h"
#include "core/engine.h"
#include "core/gameMap.h"

ActionResult MassGrabAction::perform(Engine& engine, Entity* entity) {
  // Get Player's Inventory and GameMap
  Inventory& inv = entity->getComponent<Inventory>();
  GameMap& map = engine.getCurrentMap();

  // One by one, move the items into the inventory
  for (auto e : this->itemsToGrab) {
    std::unique_ptr<Entity> item = map.extractEntity(e);
    std::string name = item->getName();
    ItemAddResult reject = inv.addItem(std::move(item));

    if (reject.result) {
      // We couldn't move the item, fail
      GameWorld& world = engine.getGameWorld();
      world.addEntityToMap(std::move(reject.result));
      return ActionResult(false, reject.message);
    }
  }
  std::string plural = this->itemsToGrab.size() == 1 ? "." : "s.";
  return ActionResult(true, "You picked up the item" + plural);
}
