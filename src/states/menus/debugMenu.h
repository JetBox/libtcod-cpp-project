#pragma once

#include "core/debug.h"
#include "listMenu.h"

class DebugMenu : public ListMenu {
 public:
  DebugMenu(Engine* engine) {
    this->engine = engine;
    this->setXOffset(2);
  }

  void buildRows(Engine& engine) final;
  virtual void printRow(tcod::Console* console, int i, int& itemCounter, Position p, int w) override;
  bool onSelect(Engine& engine, int index = 0) override;

  virtual bool onLeft(Engine& /* engine */, int /* index */) override { return true; }
  virtual bool onRight(Engine& /* engine */, int /* index */) override { return true; }

 private:
  Engine* engine = nullptr;
};
