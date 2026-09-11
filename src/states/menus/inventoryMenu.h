#pragma once

#include <functional>
#include <vector>

#include "entityListMenu.h"

class InventoryMenu : public EntityListMenu {
 public:
  using ActionFn = std::function<bool(Engine&, Entity*)>;

  InventoryMenu(
      std::string title,
      std::vector<Entity*> itemList,
      ActionFn onChoose,
      std::function<bool(Entity*)> filter = [](Entity*) { return true; });

 protected:
  bool onSelect(Engine& engine, int itemIndex) override;
  virtual bool onLeft(Engine& /*engine*/, int /*index*/) override { return true; };
  virtual bool onRight(Engine& /*engine*/, int /*index*/) override { return true; };

 private:
  ActionFn onChoose;
};

extern std::function<bool(Engine& engine, Entity* entity)> inspectMenuOnChoose;
extern std::function<bool(Engine& engine, Entity* entity)> wearMenuOnChoose;
extern std::function<bool(Engine& engine, Entity* entity)> dropMenuOnChoose;
