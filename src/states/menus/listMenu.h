#pragma once

#include <string>
#include <vector>

#include "core/engine.h"
#include "states/gameState.h"

struct MenuRow {
  std::string text;
  bool isHeader = false;
};

class ListMenu : public GameState {
 public:
  bool handleEvent(Engine& engine, SDL_Event* event) override;
  void render(Engine& engine, tcod::Console* console) override;
  bool renderStateBelow() const override { return true; }

 protected:
  virtual bool onSelect(Engine& engine, int index = 0) = 0;
  virtual bool onLeft(Engine& engine, int index = 0) = 0;
  virtual bool onRight(Engine& engine, int index = 0) = 0;
  virtual bool pressLetter(Engine& engine, int itemIdx);

  virtual void buildRows(Engine& engine) = 0;

  void addHeader(const std::string& text) { rows.push_back({text, true}); }
  void addItem(const std::string& text) { rows.push_back({text, false}); }
  void clearRows() { rows.clear(); }
  virtual void printRow(tcod::Console* console, int i, int& itemCounter, Position p, int width);

  bool isHiglighted(int row);
  static char letterFor(int index);
  static int indexForLetter(char c);
  bool isSelectable(int rowIndex) const;
  int itemIndexOfRow(int rowIndex) const;
  int rowOfItemIndex(int itemIndex) const;
  int firstSelectableFrom(int rowIndex, int dir) const;
  void clampHighlightToSelectable();

  bool isOverlayMenu() const override { return true; }

  void setSize(int w, int h) {
    this->width = w;
    this->height = h;
  }

  void setXOffset(int x) { this->x_offset = x; }

  std::string title;
  std::string footer = "up/down/8/2 move | Enter select | Esc back";
  std::vector<MenuRow> rows;

 private:
  int width = 40;
  int height = 30;
  int highlight = 0;
  int scrollTop = 0;
  int x_offset = (VIEW_WIDTH - 40) / 2;
  bool rebuildRows = true;
};
