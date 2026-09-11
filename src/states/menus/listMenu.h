#pragma once

#include <string>
#include <vector>

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
  virtual void printRow(tcod::Console* console, int i, int& itemCounter, Position p);

  bool isHiglighted(int row);
  static char letterFor(int index);
  static int indexForLetter(char c);
  bool isSelectable(int rowIndex) const;
  int itemIndexOfRow(int rowIndex) const;
  int rowOfItemIndex(int itemIndex) const;
  int firstSelectableFrom(int rowIndex, int dir) const;
  void clampHighlightToSelectable();

  void setSize(int w, int h) {
    this->width = w;
    this->height = h;
  }

  std::string title;
  std::string footer = "up/down/8/2 move | Enter select | Esc back";
  std::vector<MenuRow> rows;

 private:
  int width = 40;
  int height = 0;
  int highlight = 0;
  int scrollTop = 0;
  bool rebuildRows = true;
};
