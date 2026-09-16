#include "entity.h"

#include <memory>

#include "colors.h"
#include "components/ai/aiComponent.h"
#include "components/component.h"
#include "components/explorable.h"

void Entity::move(Direction dir) { this->pos = this->pos + dir; }

void Entity::setComponentsOwner() {
  for (auto& [key, value] : this->components) {
    value->setEntity(this);
  }
}

std::unique_ptr<Entity> spawnOrc(Position p) {
  std::unique_ptr<Entity> orc = std::make_unique<Entity>(p, 'o', ORC_GREEN, "orc", RenderOrder::Actor, true);
  orc->addComponent<Explorable>(ORC_GREEN_DARK, true);
  orc->addComponent<AIComponent>();
  orc->addComponent<Speed>();
  return orc;
}

std::unique_ptr<Entity> spawnTroll(Position p) {
  std::unique_ptr<Entity> troll = std::make_unique<Entity>(p, 'T', TROLL_GREEN, "orc", RenderOrder::Actor, true);
  troll->addComponent<Explorable>(ORC_GREEN_DARK, true);
  troll->addComponent<AIComponent>();
  troll->addComponent<Speed>();
  return troll;
}
