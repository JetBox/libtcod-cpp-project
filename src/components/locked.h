#pragma once

#include "component.h"

class Locked : public BaseComponent {
 public:
  Locked(bool isLocked = true) : isLocked(isLocked) {}

  bool getIsLocked() { return this->isLocked; }
  void unlock() { this->isLocked = false; }

 private:
  bool isLocked;
};
