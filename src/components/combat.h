#pragma once

#include <libtcod/mersenne_types.h>

#include <algorithm>
#include <cassert>
#include <libtcod.hpp>

#include "component.h"
#include "math.h"

static constexpr int DEFAULT_HP_REGEN = 1;
static constexpr int DEFAULT_MP_REGEN = 5;

class Fighter : public BaseComponent {
 public:
  Fighter(int maxHP = 100, int maxMP = 100, int hpRegen = DEFAULT_HP_REGEN, int mpRegen = DEFAULT_MP_REGEN) {
    this->maxHP = maxHP;
    this->maxMP = maxMP;
    this->hp = maxHP;
    this->mp = maxMP;
    this->hpRegen = hpRegen;
    this->mpRegen = mpRegen;
  }

  void setHP(int value) { this->hp = std::max(0, std::min(this->maxHP, value)); }
  void setMP(int value) { this->mp = std::max(0, std::min(this->maxMP, value)); }

  void takeDamage(int incomingDamage);

  bool isAlive() { return this->hp > 0; }

  int getHP() { return this->hp; }
  int getMaxHP() { return this->maxHP; }
  int getMP() { return this->mp; }
  int getMaxMP() { return this->maxMP; }

  int getBasePower() { return this->basePower; }
  int getBaseDodge() { return this->baseDodge; }
  int getBaseArmor() { return this->baseArmor; }

 private:
  int maxHP = 0;
  int hp = 0;
  int hpRegen = 0;
  int maxMP = 0;
  int mp = 0;
  int mpRegen = 0;

  // fighting
  int basePower = 3;
  int baseDodge = 3;
  int baseArmor = 0;
};

int getPower(Entity* e);
int getDodge(Entity* e);
int getArmor(Entity* e);