#pragma once

#include "component.h"

class Speed : public BaseComponent {
 public:
  static constexpr int DEFAULT_ENERGY_COST = 100;
  static constexpr int ENERGY_NEEDED_TO_ACT = 100;

  Speed(int speed = DEFAULT_ENERGY_COST) : speed(speed) {}

  bool gainEnergy() {
    this->energy += this->speed;
    return energy >= ENERGY_NEEDED_TO_ACT;
  }

  int getEnergy() const { return this->energy; }

  int canAct() const { return this->energy >= ENERGY_NEEDED_TO_ACT; }
  void spend(int cost) { this->energy -= cost; }

  int getSpeed() { return this->speed; }
  void setSpeed(int s) { this->speed = s; }

 private:
  int energy = 0;
  int speed;
};
