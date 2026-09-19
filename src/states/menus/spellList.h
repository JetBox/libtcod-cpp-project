#pragma once

#include "listMenu.h"

class SpellList : public ListMenu {
 public:
  SpellList() = default;

  void buildRows(Engine& engine) final;

  bool onSelect(Engine& engine, int index = 0) override;

  virtual bool onLeft(Engine& /* engine */, int /* index */) override { return true; }
  virtual bool onRight(Engine& /* engine */, int /* index */) override { return true; }
};

bool verifyVisible(Engine& engine, Position target);
bool verifyEntity(Engine& engine, Position target);
