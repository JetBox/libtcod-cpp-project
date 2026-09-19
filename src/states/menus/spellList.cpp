#include "spellList.h"

#include <memory>
#include <string>

#include "actions/castAction.h"
#include "components/explorer.h"
#include "components/spells.h"
#include "core/engine.h"
#include "states/menus/alertBox.h"
#include "states/tileSelectionState.h"

void SpellList::buildRows(Engine& engine) {
  this->clearRows();
  this->title = "Cast which spell?";
  Spellbook& spellbook = engine.getPlayer()->getComponent<Spellbook>();
  std::vector<Spell>& spells = spellbook.getKnownSpells();
  for (auto spell : spells) {
    SpellDefinition spD = getSpellDefinition(spell);
    this->addItem(spD.name + "(" + std::to_string(spD.mpCost) + " MP)");
  }
}

bool SpellList::onSelect(Engine& engine, int index) {
  Spellbook& spellbook = engine.getPlayer()->getComponent<Spellbook>();
  std::vector<Spell>& spells = spellbook.getKnownSpells();

  if (index < 0 || index >= static_cast<int>(spells.size())) {
    return true;
  }

  Spell chosenSpell = spells.at(index);
  SpellDefinition spD = getSpellDefinition(chosenSpell);

  // For tile selection callback
  auto chosenEntityCastOnEnter = [chosenSpell](Engine& engine, Position target) -> bool {
    SpellDefinition spD = getSpellDefinition(chosenSpell);
    if (verifyEntity(engine, target) && verifyVisible(engine, target)) {
      engine.queueAction(std::make_unique<CastAction>(chosenSpell, target));
      engine.popState();
      return false;
    }
    return true;
  };

  switch (spD.targetType) {
    case SpellTargetType::CHOSEN_ENTITY:
      engine.pushState(
          std::make_unique<TileSelectionState>(
              false,
              engine.getPlayer()->getComponent<Explorer>().getNearestFighterToPlayer(engine),
              chosenEntityCastOnEnter));
      return true;
    default:
      // Do nothing lmao
      return true;
  }
}

bool verifyVisible(Engine& engine, Position target) {
  if (!engine.getPlayer()->getComponent<Explorer>().isTileVisible(target)) {
    return false;
  }
  return true;
}

bool verifyEntity(Engine& engine, Position target) {
  std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesAt(target);
  if (entities.empty()) {
    return false;
  }
  return true;
}
