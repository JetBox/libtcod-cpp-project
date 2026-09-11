#pragma once
#include <memory>
#include <vector>

#include "common/entity.h"
#include "component.h"
#include "components/body.h"

static constexpr int MAX_INVENTORY_ITEMS = 52;

struct ItemAddResult {
  ItemAddResult(std::unique_ptr<Entity> result = nullptr, std::string message = "")
      : result(std::move(result)), message(message) {}
  std::unique_ptr<Entity> result;
  std::string message;
};

class Inventory : public BaseComponent {
 public:
  Inventory(int maxItems = MAX_INVENTORY_ITEMS) { this->maxItems = maxItems; };

  std::vector<Entity*> getItems();

  int findIndexOfItem(Entity* entity);

  // Returns nullptr if successful; returns the item back if can't pick up
  ItemAddResult addItem(std::unique_ptr<Entity> item);

  Entity* getItem(int index);

  std::unique_ptr<Entity> removeItem(int index);

  bool isFull() { return static_cast<int>(items.size()) >= maxItems; }

  Entity* findEquippedItem(BodyPartKind kind);

  int getMaxItems() { return this->maxItems; }

  // Returns the index of the item
  int findItemByName(std::string name);

 private:
  int maxItems;
  std::vector<std::unique_ptr<Entity>> items;
};
