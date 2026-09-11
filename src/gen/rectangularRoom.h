#pragma once

#include "common/position.h"

struct RectangularRoom {
  RectangularRoom() = default;
  RectangularRoom(Position topLeft, Position bottomRight) : p1(topLeft), p2(bottomRight) {}
  RectangularRoom(Position topLeft, int width, int height) : p1(topLeft) { p2 = Position(p1.x + width, p1.y + height); }

  // Getters
  int x1() const { return p1.x + 1; }
  int y1() const { return p1.y + 1; }
  int x2() const { return p2.x - 1; }
  int y2() const { return p2.y - 1; }

  // Get the Position at the center of the room
  Position center();

  // Get a room that is 1-within the edges (so width-2 and height-2)
  RectangularRoom inner();

  // Determine if this room intersects with another
  bool intersects(const RectangularRoom& other);

  Position p1;
  Position p2;
};
