#include "combat.h"

#include <SDL3/SDL_log.h>

#include <cassert>

#include "common/entity.h"

void Fighter::takeDamage(int incomingDamage) {
  int newHP = this->hp - incomingDamage;
  this->setHP(newHP);
}

int getPower(Entity* e) {
  if (!e->hasComponent<Fighter>()) {
    return -1;
  }

  Fighter& fighter = e->getComponent<Fighter>();
  return fighter.getBasePower();
}

int getDodge(Entity* e) {
  if (!e->hasComponent<Fighter>()) {
    return -1;
  }

  Fighter& fighter = e->getComponent<Fighter>();
  return fighter.getBaseDodge();
}

int getArmor(Entity* e) {
  if (!e->hasComponent<Fighter>()) {
    return -1;
  }

  Fighter& fighter = e->getComponent<Fighter>();
  return fighter.getBaseArmor();
}
