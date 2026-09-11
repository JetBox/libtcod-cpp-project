#pragma once

#include <string>

#include "body.h"
#include "component.h"

class Item : public BaseComponent {
 public:
  Item(std::string name, std::string description, int maxStackSize = 0)
      : name(name), description(description), maxStackSize(maxStackSize) {}

  std::string getName() { return this->name; }
  std::string getDescription() { return this->description; }

  int getStackSize() { return this->stackSize; }
  int getMaxStackSize() { return this->maxStackSize; }

  // Returns true if successful, returns false if not
  bool add(int n);
  bool remove(int n);

 private:
  std::string name;
  std::string description;

  int maxStackSize = 1;
  int stackSize = 1;
};

class Equippable : public BaseComponent {
 public:
  Equippable(BodyPartKind slot, bool cursed = false, bool equipped = false)
      : slot(slot), cursed(cursed), equipped(equipped) {}

  void setIsCursed(bool val) { this->cursed = val; }
  BodyPartKind getSlot() { return this->slot; }
  bool getIsCursed() { return this->cursed; }
  bool getIsEquipped() { return this->equipped; }
  void setIsEquipped(bool isEquipped) { this->equipped = isEquipped; }

 private:
  BodyPartKind slot;
  bool cursed = false;
  bool equipped = false;
};

class Consumable : public BaseComponent {
  // TODO: implement item that does stuff
};
