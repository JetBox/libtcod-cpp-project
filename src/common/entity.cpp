#include "entity.h"

#include <memory>

#include "colors.h"
#include "components/ai/aiComponent.h"
#include "components/combat.h"
#include "components/explorable.h"

void Entity::move(Direction dir) { this->pos = this->pos + dir; }

void Entity::clearEntity() {
  for (auto& [key, val] : this->components) {
    val->setEntity(nullptr);
  }
  this->components.clear();
}

std::unique_ptr<Entity> spawnOrc(Position p) {
  std::unique_ptr<Entity> orc = std::make_unique<Entity>(p, 'o', ORC_GREEN, "orc", RenderOrder::Actor, true);
  orc->addComponent<Explorable>(ORC_GREEN_DARK, true);
  orc->addComponent<AIComponent>();
  orc->addComponent<Speed>();
  orc->addComponent<Fighter>(3);
  return orc;
}

std::unique_ptr<Entity> spawnTroll(Position p) {
  return std::make_unique<Entity>(p, 'T', TROLL_GREEN, "orc", RenderOrder::Actor, true);
}
