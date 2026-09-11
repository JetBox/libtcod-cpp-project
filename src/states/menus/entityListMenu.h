#pragma once

#include "listMenu.h"

class EntityListMenu : public ListMenu {
 protected:
  void buildRows(Engine& engine) final;
  std::vector<Entity*> itemList;
  std::vector<Entity*> itemOrder;
  std::function<bool(Entity*)> filter;
};

extern std::function<bool(Entity*)> filterForEquippable;
