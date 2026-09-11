#pragma once

#include "entityListMenu.h"

class ItemSelectMenu : public EntityListMenu {
 public:
  ItemSelectMenu(
      std::string title,
      std::vector<Entity*> itemList,
      std::function<bool(Engine&, std::vector<Entity*>)> onChoose,
      std::function<bool(Entity*)> filter = [](Entity*) { return true; });

 protected:
  bool onSelect(Engine& engine, int itemIndex) override;
  virtual bool onLeft(Engine& /*engine*/, int /*index*/) override;
  virtual bool onRight(Engine& /*engine*/, int /*index*/) override;
  void printRow(tcod::Console* console, int i, int& itemCounter, Position p) override;
  bool pressLetter(Engine& engine, int itemIdx) override;

 private:
  std::function<bool(Engine&, std::vector<Entity*>)> onChoose;
  std::vector<bool> selectedItems;
};
