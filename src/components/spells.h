#pragma once

#include <string>
#include <vector>

#include "component.h"

enum class Spell { MAGIC_MISSILE, KNOCK };

enum class SpellType { DIRECT_DAMAGE, KNOCK, UNKNOWN };

enum SpellTargetType { SELF, NEAREST, ALL_VISIBLE, CHOSEN_ENTITY, CHOSEN_SQUARE, DIRECTION, NONE };

struct SpellDefinition {
  std::string name;
  int mpCost;
  int basePower;  // use -1 for spells with no power
  SpellType spellType;
  SpellTargetType targetType;

  SpellDefinition(std::string name, int mpCost, int basePower, SpellType spellType, SpellTargetType targetType)
      : name(name), mpCost(mpCost), basePower(basePower), spellType(spellType), targetType(targetType) {}
};

class Spellbook : public BaseComponent {
 public:
  Spellbook() = default;

  // Returns false if already knows spell
  bool addSpell(Spell spell);
  bool knowsSpell(Spell spell);

  std::vector<Spell>& getKnownSpells();

 private:
  std::vector<Spell> knownSpells;
};

SpellDefinition getSpellDefinition(Spell spell);
