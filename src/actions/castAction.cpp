#include "castAction.h"

#include "action.h"
#include "components/combat.h"

ActionResult CastAction::perform(Engine& engine, Entity* entity) {
  // First, get the spell definition
  SpellDefinition spDef = getSpellDefinition(this->spell);

  // Make sure caster has enough MP to cast
  if (entity->hasComponent<Fighter>()) {
    return ActionResult(false, "ERROR: Need Fighter Component to cast spells.");
  }
  Fighter& fighter = entity->getComponent<Fighter>();
  if (fighter.getMP() < spDef.mpCost) {
    return ActionResult(false, "Not enough MP!");
  }

  // Switch based on spell type
  switch (spDef.spellType) { case SpellType::DAMAGE: }
}
