#pragma once

#include <libtcod/mersenne_types.h>

#include <algorithm>
#include <cassert>
#include <libtcod.hpp>

#include "component.h"
#include "math.h"

class BaseStats {
 public:
  BaseStats() = default;

  BaseStats(int str, int agi, int in) : strength(str), agility(agi), intelligence(in) {}
  int getStrength() { return this->strength; }
  int getIntelligence() { return this->intelligence; }
  // int getPerception() { return this->perception; }

  void setStrength(int str) { this->strength = std::max(std::min(str, MAX_STAT), MIN_STAT); }
  void setAgility(int agi) { this->agility = std::max(std::min(agi, MAX_STAT), MIN_STAT); }
  void setIntelligence(int in) { this->intelligence = std::max(std::min(in, MAX_STAT), MIN_STAT); }
  // void setPerception(int per) { this->perception = std::max(std::min(per, MAX_STAT), MIN_STAT); }

 private:
  static constexpr int MAX_STAT = 9;
  static constexpr int MIN_STAT = 0;
  int strength;
  int agility;
  int intelligence;
  // int perception;
};

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

  void setBaseStats(int str, int agi, int in) { this->stats = BaseStats(str, agi, in); }
  BaseStats getBaseStats() { return this->stats; }

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
  BaseStats stats;
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

bool makeStatCheck(int stat);
