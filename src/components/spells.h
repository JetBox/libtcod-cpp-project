#pragma once

#include <set>
#include <string>

#include "component.h"

enum class Spell { MAGIC_MISSILE, KNOCK };

enum class SpellType { DAMAGE, KNOCK, UNKNOWN };

enum TargetType { SELF, NEAREST, ALL_VISIBLE, CHOSEN_ENTITY, CHOSEN_SQUARE, DIRECTION, NONE };

struct SpellDefinition {
  std::string name;
  int mpCost;
  int basePower;  // use -1 for spells with no power
  SpellType spellType;
  TargetType targetType;

  SpellDefinition(std::string name, int mpCost, int basePower, SpellType spellType, TargetType targetType)
      : name(name), mpCost(mpCost), basePower(basePower), spellType(spellType), targetType(targetType) {}
};

class Spellbook : public BaseComponent {
 public:
  Spellbook() = default;

  // Returns false if already knows spell
  bool addSpell(Spell spell);
  bool knowsSpell(Spell spell);

  std::set<Spell> getKnownSpells();

 private:
  std::set<Spell> knownSpells;
};

SpellDefinition getSpellDefinition(Spell spell);
