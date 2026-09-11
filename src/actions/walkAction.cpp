#include "walkAction.h"

#include "action.h"
#include "common/tiles.h"
#include "components/item.h"
#include "components/openable.h"
#include "core/engine.h"
#include "meleeAction.h"
#include "openAction.h"

ActionResult WalkAction::perform(Engine& engine, Entity* entity) {
  Position endPosition = entity->getPosition() + this->dir;
  GameMap& map = engine.getCurrentMap();
  if (map.inBounds(endPosition) && map.getTileAt(endPosition).isWalkable) {
    // check if we ran into an enemy
    Entity* e = map.getBlockingEntity(endPosition);
    if (!e) {
      // No blocking entity, we can move here
      entity->move(this->dir);

      // if we landed on an item, alert the player of what they stepped on
      std::string message = "";
      if (engine.getPlayer() == entity) {
        auto eList = map.getEntitiesAt(endPosition, entity);

        // Filter out non-Items
        std::vector<Entity*> iList;
        for (auto ent : eList) {
          if (ent->hasComponent<Item>()) {
            iList.push_back(ent);
          }
        }

        if (!iList.empty()) {
          // check that the eList has an item in it
          bool hasItem = false;
          for (auto ent : iList) {
            if (ent->hasComponent<Item>()) {
              hasItem = true;
            }
          }
          if (hasItem) {
            message = "You stepped on ";
            for (unsigned long long i = 0; i < iList.size(); i++) {
              if (i != 0) {
                message += ", ";
              } else if (iList.size() > 1 && i == iList.size() - 1) {
                message += ", and ";
              }
              message += iList[i]->getName();
            }
            message += ".";
          }
        }
      }

      return ActionResult(true, message);
    } else {
      // There is a blocking entity, check what components it has
      if (e->hasComponent<Openable>()) {
        return ActionResult(std::make_unique<OpenAction>(this->dir));
      }
      return ActionResult(std::make_unique<MeleeAction>(this->dir, 0));
    }
  } else {
    return ActionResult(false, Message("Ouch!"));
  }
}
