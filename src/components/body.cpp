#include "body.h"

BodyPart* Body::find(BodyPartKind kind) {
  for (BodyPart& p : this->parts) {
    if (p.partKind == kind) {
      return &p;
    }
  }
  return nullptr;
}
Body::Body() {
  parts.emplace_back(BodyPartKind::HEAD, "head");
  parts.emplace_back(BodyPartKind::TORSO, "square body");
  parts.emplace_back(BodyPartKind::ARMS, "arms");
  parts.emplace_back(BodyPartKind::LEGS, "legs");
  parts.emplace_back(BodyPartKind::FEET, "feet");
}
