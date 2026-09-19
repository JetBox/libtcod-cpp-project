#pragma once

#include "action.h"
#include "common/position.h"
#include "components/spells.h"

class CastAction : public Action {
 public:
  CastAction(Spell spell, Position targetPosition) : spell(spell), targetPosition(targetPosition) {}

  ActionResult perform(Engine& engine, Entity* entity) override;

  std::vector<Entity*> resolveTargets(Engine& engine, SpellDefinition spD, Entity* caster);

 private:
  Spell spell;
  Position targetPosition = {-1, -1};
};
