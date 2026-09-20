#include "castAction.h"

#include "action.h"
#include "components/combat.h"
#include "components/locked.h"
#include "components/openable.h"
#include "components/spells.h"
#include "core/engine.h"

ActionResult CastAction::perform(Engine& engine, Entity* entity) {
  // First, get the spell definition
  SpellDefinition spDef = getSpellDefinition(this->spell);

  // Make sure caster has enough MP to cast
  if (!entity->hasComponent<Fighter>()) {
    return ActionResult(false, "ERROR: Need Fighter Component to cast spells.");
  }
  Fighter& fighter = entity->getComponent<Fighter>();
  if (fighter.getMP() < spDef.mpCost) {
    return ActionResult(false, "Not enough MP!");
  }
  fighter.setMP(fighter.getMP() - spDef.mpCost);

  std::vector<Entity*> targets = this->resolveTargets(engine, spDef, entity);

  // Resolve effects
  switch (spDef.spellType) {
    case SpellType::KNOCK: {
      if (targets.empty()) {
        return ActionResult(false, "Your spell fizzles.");
      }
      Entity* door = targets.at(0);
      Openable& openable = door->getComponent<Openable>();
      if (!openable.isClosed()) {
        // Already open, spell fizzles
        return ActionResult(true, "Your spell fizzles.");
      }
      if (door->hasComponent<Locked>()) {
        door->getComponent<Locked>().unlock();
      }
      openable.toggleOpen(engine, entity);
      return ActionResult(true, "Knock!");
    }
    case SpellType::DIRECT_DAMAGE: {
      if (targets.empty()) {
        return ActionResult(false, "Invalid target.");
      }
      Entity* target = targets.at(0);
      target->getComponent<Fighter>().takeDamage(spDef.basePower);
      return ActionResult(true, "You hit the " + target->getName() + "!");
    }
    default:
      // Do nothing lmao
      break;
  }
}

std::vector<Entity*> CastAction::resolveTargets(Engine& engine, SpellDefinition spD, Entity* caster) {
  std::vector<Entity*> entities;
  std::vector<Entity*> chosen;
  switch (spD.targetType) {
    case SpellTargetType::SELF:
      entities.push_back(caster);
      break;
    case SpellTargetType::CHOSEN_ENTITY:
      chosen = engine.getCurrentMap().getEntitiesAt(this->targetPosition);
      // Get fighter unless knock
      for (auto e : chosen) {
        if (spD.spellType == SpellType::KNOCK && e->hasComponent<Openable>()) {
          entities.push_back(e);
          break;
        } else if (e->hasComponent<Fighter>()) {
          entities.push_back(e);
          break;
        }
      }
      break;
    default:
      break;
  }

  return entities;
}
