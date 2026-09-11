#pragma once

#include <algorithm>

#include "component.h"

class Vitals : public BaseComponent {
 public:
  Vitals() = default;
  Vitals(int h, int f) : hunger(h), fatigue(f) {}

  static constexpr int MAX_HUNGER = 100;
  static constexpr int MAX_FATIGUE = 100;

  static constexpr int HUNGER_TIMER_MAX = 100;
  static constexpr int FATIGUE_TIMER_MAX = 100;

  static constexpr int FATIGUE_EASY = 50;
  static constexpr int FATIGUE_NORMAL = 100;
  static constexpr int FATIGUE_HARD = 250;

  int getHunger() { return this->hunger; }
  int getFatigue() { return this->fatigue; }

  void addHunger(int h) { this->hunger = std::clamp(this->hunger + h, 0, 100); }
  void addFatigue(int f) { this->fatigue = std::clamp(this->fatigue + f, 0, 100); }

  void expendFatigue(int amount) { this->fatigueTimer += amount; }

  void updateTick() {
    // Increment the timer's by 1
    this->hungerTimer++;
    this->fatigueTimer++;

    // Reset Hunger
    if (this->hungerTimer >= HUNGER_TIMER_MAX) {
      int difference = this->hungerTimer / HUNGER_TIMER_MAX;
      this->addHunger(-1 * difference);
      while (hungerTimer >= HUNGER_TIMER_MAX) {
        hungerTimer -= HUNGER_TIMER_MAX;
      }
    }

    // Reset Fatigue
    if (this->fatigueTimer >= FATIGUE_TIMER_MAX) {
      int difference = this->fatigueTimer / FATIGUE_TIMER_MAX;
      this->addFatigue(difference);
      while (fatigueTimer >= FATIGUE_TIMER_MAX) {
        fatigueTimer -= FATIGUE_TIMER_MAX;
      }
    }
  }

 private:
  int hunger;
  int fatigue;

  int hungerTimer = 0;
  int fatigueTimer = 0;
};
