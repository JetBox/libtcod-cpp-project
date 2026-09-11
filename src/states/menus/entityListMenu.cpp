#include "entityListMenu.h"

#include <string>

#include "components/body.h"
#include "components/item.h"

namespace {
enum class Category { WEAPON, ARMOR, ACCESSORY, POTION, MISC };

Category categoryOf(Entity* e) {
  if (e->hasComponent<Equippable>()) {
    BodyPartKind slot = e->getComponent<Equippable>().getSlot();
    if (slot == BodyPartKind::ARMS) {
      return Category::WEAPON;
    }
    return Category::ARMOR;
  }
  if (e->hasComponent<Consumable>()) {
    return Category::POTION;
  }
  return Category::MISC;
}

const char* categoryHeader(Category c) {
  switch (c) {
    case Category::WEAPON:
      return "Weapons";
    case Category::ARMOR:
      return "Armor";
    case Category::ACCESSORY:
      return "Accessories";
    case Category::POTION:
      return "Potions";
    case Category::MISC:
      return "Miscellaneous";
  }
  return "Unknown";
}

int slotRank(Entity* e) {
  if (e->hasComponent<Equippable>()) {
    return static_cast<int>(e->getComponent<Equippable>().getSlot());
  }
  return 0;
}

std::string itemName(Entity* e) {
  std::string name;
  if (e->hasComponent<Item>()) {
    name = e->getComponent<Item>().getName();
  }
  name = e->getName();

  // Add if it's equipped
  if (e->hasComponent<Equippable>()) {
    if (e->getComponent<Equippable>().getIsEquipped()) {
      name += " (equipped)";
    }
  }

  // Alternatively, show if there's more than one
  if (e->hasComponent<Item>() && e->getComponent<Item>().getStackSize() > 1) {
    name += " x" + std::to_string(e->getComponent<Item>().getStackSize());
  }
  return name;
}

}  // namespace

void EntityListMenu::buildRows(Engine& /*engine*/) {
  this->clearRows();
  this->itemOrder.clear();

  // Filter out the unwated items
  std::vector<Entity*> sorted;
  for (auto e : this->itemList) {
    ;
    if (this->filter(e)) {
      sorted.push_back(e);
    }
  }

  std::sort(sorted.begin(), sorted.end(), [](Entity* a, Entity* b) {
    Category ca = categoryOf(a), cb = categoryOf(b);
    if (ca != cb) return ca < cb;
    if (ca == Category::ARMOR && slotRank(a) != slotRank(b)) {
      return slotRank(a) < slotRank(b);
    }
    return itemName(a) < itemName(b);
  });

  bool multipleCategories = false;
  if (!sorted.empty()) {
    Category firstCat = categoryOf(sorted.front());
    for (Entity* e : sorted) {
      if (categoryOf(e) != firstCat) {
        multipleCategories = true;
        break;
      }
    }
  }

  Category current = Category::MISC;
  bool first = true;
  for (Entity* e : sorted) {
    Category c = categoryOf(e);
    if (multipleCategories && (first || c != current)) {
      this->addHeader(categoryHeader(c));
      current = c;
    }
    first = false;
    this->addItem(itemName(e));
    this->itemOrder.push_back(e);
  }
}

std::function<bool(Entity*)> filterForEquippable = [](Entity* entity) { return entity->hasComponent<Equippable>(); };
