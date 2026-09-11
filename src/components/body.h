#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/entity.h"
#include "component.h"

// Head can be ears, horns, etc.
enum class BodyPartKind { HEAD, TORSO, ARMS, FINGERS, BACK, LEGS, FEET };

struct BodyPart {
  BodyPartKind partKind;
  std::string name;

  bool equipEverPossible = true;
  bool canEquip = true;

  BodyPart() = default;
  BodyPart(BodyPartKind partKind, std::string name, bool canEquip = true)
      : partKind(partKind), name(name), canEquip(canEquip) {}
};

class Body : public BaseComponent {
 public:
  Body();

  BodyPart* find(BodyPartKind kind);

  std::vector<BodyPart>& getParts() { return parts; }

 private:
  std::vector<BodyPart> parts;
};
