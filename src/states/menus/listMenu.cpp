#include "listMenu.h"

#include <SDL3/SDL_keycode.h>

#include <optional>

#include "common/colors.h"
#include "core/engine.h"
#include "core/windowFrame.h"

char ListMenu::letterFor(int index) {
  return index < 26 ? static_cast<char>('a' + index) : static_cast<char>('A' + (index - 26));
}

int ListMenu::indexForLetter(char c) {
  if (c >= 'a' && c <= 'z') return c - 'a';
  if (c >= 'A' && c <= 'Z') return 26 + (c - 'A');
  return -1;
}

bool ListMenu::isSelectable(int rowIndex) const {
  return rowIndex >= 0 && rowIndex < static_cast<int>(rows.size()) && !rows[rowIndex].isHeader;
}

int ListMenu::firstSelectableFrom(int rowIndex, int dir) const {
  int n = static_cast<int>(rows.size());
  for (int i = rowIndex; i >= 0 && i < n; i += dir) {
    if (!rows[i].isHeader) return i;
  }
  return -1;
}

int ListMenu::itemIndexOfRow(int rowIndex) const {
  if (!isSelectable(rowIndex)) return -1;
  int count = 0;
  for (int i = 0; i < rowIndex; i++) {
    if (!rows[i].isHeader) count++;
  }
  return count;
}

int ListMenu::rowOfItemIndex(int itemIndex) const {
  int count = 0;
  for (int i = 0; i < static_cast<int>(rows.size()); i++) {
    if (rows[i].isHeader) continue;
    if (count == itemIndex) return i;
    count++;
  }
  return -1;
}

void ListMenu::clampHighlightToSelectable() {
  if (rows.empty()) {
    this->highlight = 0;
    return;
  }
  if (!isSelectable(this->highlight)) {
    int down = firstSelectableFrom(this->highlight, +1);
    int up = firstSelectableFrom(this->highlight, -1);
    highlight = (down != -1) ? down : (up != -1 ? up : 0);
  }
}

bool ListMenu::handleEvent(Engine& engine, SDL_Event* event) {
  if (event->type != SDL_EVENT_KEY_DOWN) {
    return true;
  }

  const SDL_Keycode key = event->key.key;
  int next = -1;
  int itemIdx = -1;

  switch (key) {
    case SDLK_ESCAPE:
      return false;
    case SDLK_UP:
    case SDLK_KP_8:
      next = firstSelectableFrom(this->highlight - 1, -1);
      if (next != -1) {
        highlight = next;
      }
      return true;
    case SDLK_DOWN:
    case SDLK_KP_2:
      next = firstSelectableFrom(this->highlight + 1, +1);
      if (next != -1) {
        highlight = next;
      }
      return true;
    case SDLK_KP_4:
    case SDLK_LEFT:
      itemIdx = itemIndexOfRow(this->highlight);
      if (itemIdx >= 0) {
        return onLeft(engine, itemIdx);
      }
      return true;
    case SDLK_KP_6:
    case SDLK_RIGHT:
      itemIdx = itemIndexOfRow(this->highlight);
      if (itemIdx >= 0) {
        return onRight(engine, itemIdx);
      }
      return true;
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      itemIdx = itemIndexOfRow(this->highlight);
      if (itemIdx >= 0) {
        return onSelect(engine, itemIdx);
      }
      return true;
    default:
      break;
  }

  itemIdx = indexForLetter(static_cast<char>(key));
  return this->pressLetter(engine, itemIdx);
}

bool ListMenu::pressLetter(Engine& engine, int itemIdx) {
  if (itemIdx >= 0) {
    int row = rowOfItemIndex(itemIdx);
    if (row != -1) {
      this->highlight = row;
      return onSelect(engine, itemIdx);
    }
  }
  return true;
}

void ListMenu::render(Engine& engine, tcod::Console* console) {
  if (this->rebuildRows) {
    this->buildRows(engine);
    this->clampHighlightToSelectable();
  }

  int rowCount = static_cast<int>(this->rows.size());
  int w = this->width;
  int h = (this->height > 0) ? this->height : rowCount + 4;
  int x = (VIEW_WIDTH - w) / 2;
  int y = (VIEW_HEIGHT - h) / 2;

  WindowFrame frame(x, y, w, h, true);
  frame.setTitle(title);
  frame.setFooter(footer);
  auto [cx, cy, cw, ch] = frame.draw(*console);

  if (rows.empty()) {
    tcod::print(*console, {cx, cy}, "(empty)", WHITE, std::nullopt);
    return;
  }

  const int visibleRows = ch;

  if (this->highlight < scrollTop) {
    scrollTop = this->highlight;
  } else if (this->highlight >= scrollTop + visibleRows) {
    scrollTop = this->highlight - visibleRows + 1;
  }

  int maxScroll = std::max(0, rowCount - visibleRows);
  scrollTop = std::max(0, std::min(scrollTop, maxScroll));

  int itemCounter = 0;
  for (int i = 0; i < scrollTop; i++) {
    if (!this->rows[i].isHeader) itemCounter++;
  }

  int end = std::min(rowCount, scrollTop + visibleRows);
  for (int i = scrollTop; i < end; ++i) {
    this->printRow(console, i, itemCounter, Position(cx, cy + i));
  }
}

void ListMenu::printRow(tcod::Console* console, int i, int& itemCounter, Position p) {
  MenuRow& row = this->rows[i];
  if (row.isHeader) {
    tcod::print(*console, {p.x, p.y}, row.text, WHITE, std::nullopt);
    return;
  }

  const char letter = this->letterFor(itemCounter);
  const bool selected = this->isHiglighted(i);
  const std::string marker = selected ? "> " : "  ";
  const std::string line = marker + "(" + letter + ") " + row.text;
  const auto fg = selected ? RED : WHITE;

  tcod::print(*console, {p.x, p.y}, line, fg, std::nullopt);
  itemCounter++;
}

bool ListMenu::isHiglighted(int row) { return row == this->highlight; }
