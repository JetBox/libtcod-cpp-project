#include "item_factory.h"

#include <memory>

#include "common/colors.h"
#include "common/entity.h"
#include "components/item.h"

std::string getEquipName(BodyPartKind kind) {
  switch (kind) {
    case BodyPartKind::HEAD:
      return "helmet";
    case BodyPartKind::ARMS:
      return "longsword";
    case BodyPartKind::FEET:
      return "boots";
    case BodyPartKind::LEGS:
      return "grieves";
    case BodyPartKind::BACK:
      return "cape";
    case BodyPartKind::TORSO:
      return "mail";
    default:
      return "accessory";
  }
}

BodyPartKind getBodyPartKind(TCODRandom* rng, std::vector<BodyPartKind> arr) {
  int index = rng->getInt(0, arr.size() - 1);
  return arr[index];
}

std::string getAdjective(TCODRandom* rng, std::vector<std::string> arr) {
  int index = rng->getInt(0, arr.size() - 1);
  return arr[index];
}

char getEquipItemChar(BodyPartKind kind) {
  switch (kind) {
    case BodyPartKind::HEAD:
    case BodyPartKind::TORSO:
    case BodyPartKind::LEGS:
    case BodyPartKind::FEET:
      return '[';
    case BodyPartKind::ARMS:
      return '{';
    default:
      return '?';
  }
}

std::unique_ptr<Entity> getRandomEquipment(TCODRandom* rng, Position p) {
  BodyPartKind kind = getBodyPartKind(rng, kinds);
  std::string adjective = getAdjective(rng, adjectives);
  std::string name = adjective + " " + getEquipName(kind);
  char c = getEquipItemChar(kind);

  std::unique_ptr<Entity> e = std::make_unique<Entity>(p, c, ENTITY_WHITE, name, RenderOrder::Item, false);

  e->addComponent<Item>(name, adjective);
  e->addComponent<Equippable>(kind);

  return std::move(e);
}
