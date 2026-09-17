#include "combat.h"

#include <SDL3/SDL_log.h>

#include <cassert>

#include "common/entity.h"

void Fighter::takeDamage(int incomingDamage) {
  int newHP = this->hp - incomingDamage;
  this->setHP(newHP);
}

bool makeStatCheck(int stat) {
  TCODRandom* rng = TCODRandom::getInstance();
  int roll = rng->getInt(1, 100);
  int rollUnder = std::min(100, std::max(10, (stat * 8) + 10));

  SDL_Log("Stat Check %d/%d: %d vs %d", stat, 9, roll, rollUnder);

  return roll <= rollUnder;
}
