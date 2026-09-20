#include "debugMenu.h"

#include "components/combat.h"
#include "core/debug.h"

void DebugMenu::buildRows(Engine& /* engine*/) {
  this->clearRows();
  this->title = "Debug";

  this->addItem("Show All Entites");
  this->addItem("Show Entire Map");
  this->addItem("Kill All Enemies");
}

bool DebugMenu::onSelect(Engine& engine, int index) {
  switch (static_cast<DebugOption>(index)) {
    case DebugOption::SHOW_ALL_ENTITIES:
      engine.getDebug().toggleDebugOption(DebugOption::SHOW_ALL_ENTITIES);
      return true;
    case DebugOption::SHOW_ENTIRE_MAP:
      engine.getDebug().toggleDebugOption(DebugOption::SHOW_ENTIRE_MAP);
      return true;
    default:
      break;
  }

  if (index == 2) {
    std::vector<Entity*> entities = engine.getCurrentMap().getEntitiesWithComponent<Fighter>();
    for (auto e : entities) {
      if (e != engine.getPlayer()) {
        e->getComponent<Fighter>().setHP(0);
      }
    }
  }
}

void DebugMenu::printRow(tcod::Console* console, int i, int& itemCounter, Position p, int w) {
  MenuRow& row = this->rows[i];
  if (row.isHeader) {
    tcod::print(*console, {p.x, p.y}, row.text, WHITE, std::nullopt);
    return;
  }

  const char letter = this->letterFor(itemCounter);
  const bool selected = this->isHiglighted(i);
  const std::string marker = selected ? "> " : "  ";
  const std::string line = marker + "(" + letter + ") " + row.text;
  const auto fg = WHITE;

  tcod::print(*console, {p.x, p.y}, line, fg, std::nullopt);

  if (itemCounter < 2) {
    const std::string toggleText = engine->getDebug().getOption(static_cast<DebugOption>(itemCounter)) ? " ON" : "OFF";
    const auto toggleFg = engine->getDebug().getOption(static_cast<DebugOption>(itemCounter)) ? GREEN : RED;
    tcod::print(*console, {p.x + w - 5, p.y}, toggleText, toggleFg, std::nullopt);
  }

  itemCounter++;
}
