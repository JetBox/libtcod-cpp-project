#pragma once

#include <libtcod/mersenne_types.h>

#include <algorithm>
#include <cassert>
#include <libtcod.hpp>

#include "component.h"
#include "math.h"

class Fighter;

class BaseStats : public BaseComponent {
 public:
  BaseStats() = default;
  BaseStats(int str, int agi, int con, int in, int per)
      : strength(str), agility(agi), constitution(con), intelligence(in), perception(per) {}
  int getStrength() { return this->strength; }
  int getAgility() { return this->agility; }
  int getConstitution() { return this->constitution; }
  int getIntelligence() { return this->intelligence; }
  int getPerception() { return this->perception; }

  void setStrength(int str) { this->strength = std::max(std::min(str, MAX_STAT), MIN_STAT); }
  void setAgility(int agi) { this->agility = std::max(std::min(agi, MAX_STAT), MIN_STAT); }
  void setConstitution(int con);
  void setIntelligence(int in) { this->intelligence = std::max(std::min(in, MAX_STAT), MIN_STAT); }
  void setPerception(int per) { this->perception = std::max(std::min(per, MAX_STAT), MIN_STAT); }

 private:
  static constexpr int MAX_STAT = 9;
  static constexpr int MIN_STAT = 0;
  int strength;
  int agility;
  int constitution;
  int intelligence;
  int perception;
};

class Fighter : public BaseComponent {
 public:
  Fighter() = default;

  void setHP(int value) { this->hp = std::max(0, std::min(this->maxHP, value)); }

  int getHP() { return this->hp; }
  int getMaxHP() { return this->maxHP; }

 private:
  int maxHP = 0;
  int hp = 0;
};

bool makeStatCheck(int stat);
