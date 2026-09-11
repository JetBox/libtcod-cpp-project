#include "itemSelectMenu.h"

#include <string>

#include "common/glyphs.h"

ItemSelectMenu::ItemSelectMenu(
    std::string title,
    std::vector<Entity*> itemList,
    std::function<bool(Engine&, std::vector<Entity*>)> onChoose,
    std::function<bool(Entity*)> filter) {
  this->title = std::move(title);
  this->footer = "left/right select | Enter confirm | Esc back";
  this->onChoose = onChoose;
  this->filter = filter;
  this->itemList = itemList;
  setSize(40, 30);

  // initialize selected list
  for (int i = 0; i < static_cast<int>(this->itemList.size()); i++) {
    this->selectedItems.push_back(false);
  }
}

bool ItemSelectMenu::onRight(Engine& /*engine*/, int itemIndex) {
  this->selectedItems[itemIndex] = true;
  return true;
}

bool ItemSelectMenu::onLeft(Engine& /*engine*/, int itemIndex) {
  this->selectedItems[itemIndex] = false;
  return true;
}

bool ItemSelectMenu::onSelect(Engine& engine, int /* itemIndex */) {
  std::vector<Entity*> finalList;
  for (int i = 0; i < static_cast<int>(this->selectedItems.size()); i++) {
    if (this->selectedItems[i]) {
      finalList.push_back(this->itemList[i]);
    }
  }
  return onChoose(engine, std::move(finalList));
}

bool ItemSelectMenu::pressLetter(Engine& /* engine */, int itemIdx) {
  if (itemIdx >= 0) {
    int row = rowOfItemIndex(itemIdx);
    if (row != -1) {
      this->selectedItems[row] = !this->selectedItems[row];
    }
  }
  return true;
}

void ItemSelectMenu::printRow(tcod::Console* console, int i, int& itemCounter, Position p) {
  MenuRow& row = this->rows[i];
  if (row.isHeader) {
    tcod::print(*console, {p.x, p.y}, row.text, WHITE, std::nullopt);
    return;
  }

  const char letter = this->letterFor(itemCounter);
  const bool selected = this->isHiglighted(i);
  const std::string prepender = this->selectedItems[itemCounter] ? utf8(FILLED_CIRCLE) : utf8(OPEN_CIRCLE);
  const std::string marker = selected ? " >" : "  ";
  const std::string line = prepender + " " + marker + "(" + letter + ") " + row.text;
  const auto fg = selected ? RED : WHITE;

  tcod::print(*console, {p.x, p.y}, line, fg, std::nullopt);
  itemCounter++;
}
