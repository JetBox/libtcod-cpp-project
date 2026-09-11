#include "rectangularRoom.h"

Position RectangularRoom::center() {
  Position cp = p1 + p2;
  return Position(cp.x / 2, cp.y / 2);
}

RectangularRoom RectangularRoom::inner() {
  return RectangularRoom(Position(p1.x + 1, p1.y + 1), Position(p2.x - 1, p2.y - 1));
}

bool RectangularRoom::intersects(const RectangularRoom& other) {
  return (this->p1.x <= other.p2.x && this->p2.x >= other.p1.x && this->p1.y <= other.p2.y && this->p2.y >= other.p1.y);
}
