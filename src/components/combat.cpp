#include "combat.h"

#include <SDL3/SDL_log.h>

#include <cassert>

#include "common/entity.h"

void BaseStats::setConstitution(int con) {
  this->constitution = std::max(std::min(con, MAX_STAT), MIN_STAT);
  // Set HP/Max HP based on new constitution
  assert(this->getEntity() && "Owner must be set for BaseStats");
  assert(this->getEntity()->hasComponent<Fighter>());
  // Fighter f = this->getEntity()->getComponent<Fighter>();
  //  TODO: Set hp based on constitution
}

bool makeStatCheck(int stat) {
  TCODRandom* rng = TCODRandom::getInstance();
  int roll = rng->getInt(1, 100);
  int rollUnder = std::min(100, std::max(10, (stat * 8) + 10));

  SDL_Log("Stat Check %d/%d: %d vs %d", stat, 9, roll, rollUnder);

  return roll <= rollUnder;
}
