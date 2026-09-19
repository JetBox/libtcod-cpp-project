#include "spells.h"

bool Spellbook::addSpell(Spell spell) {
  if (this->knowsSpell(spell)) {
    return false;
  }
  this->knownSpells.push_back(spell);
  return true;
}

bool Spellbook::knowsSpell(Spell spell) {
  for (auto s : this->knownSpells) {
    if (spell == s) {
      return true;
    }
  }
  return false;
}

std::vector<Spell>& Spellbook::getKnownSpells() { return this->knownSpells; }

SpellDefinition getSpellDefinition(Spell spell) {
  switch (spell) {
    case Spell::MAGIC_MISSILE:
      return SpellDefinition("Magic Missle", 4, 4, SpellType::DIRECT_DAMAGE, SpellTargetType::CHOSEN_ENTITY);
    case Spell::KNOCK:
      return SpellDefinition("Knock", 2, -1, SpellType::KNOCK, SpellTargetType::CHOSEN_ENTITY);
    default:
      return SpellDefinition("Unknown", 0, -1, SpellType::UNKNOWN, SpellTargetType::NONE);
  }
}
