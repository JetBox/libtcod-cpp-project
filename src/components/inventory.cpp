#include "inventory.h"

#include <memory>

#include "components/item.h"

ItemAddResult Inventory::addItem(std::unique_ptr<Entity> item) {
  if (this->isFull()) {
    return ItemAddResult(std::move(item), "Inventory is full.");
  }

  // Check if an item with the same name already exists
  int searchIdx = this->findItemByName(item->getComponent<Item>().getName());

  // No duplicates, add
  if (searchIdx == -1) {
    this->items.push_back(std::move(item));
    return ItemAddResult();
  }

  // Duplicate, decide what to do
  Entity* conflict = this->getItem(searchIdx);
  Item& conflictItem = conflict->getComponent<Item>();
  if (conflictItem.getStackSize() >= conflictItem.getMaxStackSize()) {
    // Max of this stack. Only pickup if equipment
    if (conflict->hasComponent<Equippable>()) {
      this->items.push_back(std::move(item));
      return ItemAddResult();
    } else {
      return ItemAddResult(std::move(item), "You have maxed your cap on this item.");
    }
  }

  // If we're here, just increment the count and delete the item
  conflictItem.add(1);
  item.reset();
  return ItemAddResult();
}

Entity* Inventory::getItem(int index) {
  if (index < 0 || static_cast<std::size_t>(index) >= items.size()) {
    return nullptr;
  }

  return items[index].get();
}

std::unique_ptr<Entity> Inventory::removeItem(int index) {
  if (index < 0 || static_cast<std::size_t>(index) >= items.size()) {
    return nullptr;
  }

  std::unique_ptr<Entity> item = std::move(items[index]);
  items.erase(items.begin() + index);
  return item;
}

int Inventory::findIndexOfItem(Entity* entity) {
  for (int i = 0; static_cast<std::size_t>(i) < this->items.size(); i++) {
    if (items[i].get() == entity) {
      return i;
    }
  }
  return -1;
}

Entity* Inventory::findEquippedItem(BodyPartKind kind) {
  for (const auto& item : this->items) {
    if (item->hasComponent<Equippable>()) {
      auto eq = item->getComponent<Equippable>();
      if (eq.getSlot() == kind && eq.getIsEquipped()) {
        return item.get();
      }
    }
  }
  return nullptr;
}

std::vector<Entity*> Inventory::getItems() {
  std::vector<Entity*> list;
  for (auto& e : this->items) {
    list.push_back(e.get());
  }
  return std::move(list);
}

int Inventory::findItemByName(std::string name) {
  for (int i = 0; i < static_cast<int>(this->items.size()); i++) {
    if (this->items[i]->getComponent<Item>().getName() == name) {
      return i;
    }
  }
  return -1;
}
