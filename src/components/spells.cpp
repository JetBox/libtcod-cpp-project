#include "spells.h"

bool Spellbook::addSpell(Spell spell) {
  if (this->knownSpells.contains(spell)) {
    return false;
  }
  this->knownSpells.insert(spell);
  return true;
}

bool Spellbook::knowsSpell(Spell spell) { return this->knownSpells.contains(spell); }

std::set<Spell> Spellbook::getKnownSpells() { return this->knownSpells; }

SpellDefinition getSpellDefinition(Spell spell) {
  switch (spell) {
    case Spell::MAGIC_MISSILE:
      return SpellDefinition("Magic Missle", 4, 4, TargetType::CHOSEN_ENTITY);
    case Spell::KNOCK:
      return SpellDefinition("Knock", 2, -1, TargetType::CHOSEN_ENTITY);
    default:
      return SpellDefinition("Unknown", 0, -1, TargetType::NONE);
  }
}
