#include "item.h"

bool Item::add(int n) {
  int temp = this->stackSize + n;
  if (temp > this->maxStackSize) {
    return false;
  }
  this->stackSize = temp;
  return true;
}

bool Item::remove(int n) {
  int temp = this->stackSize - n;
  if (temp < 0) {
    return false;
  }
  this->stackSize = temp;
  return true;
}
