#include "position.h"

Direction getDirectionToPosition(Position p1, Position p2) {
  Position diff = p1 - p2;
  if (diff.x > 0) {
    if (diff.y == 0) {
      return Direction::WEST;
    } else if (diff.y > 0) {
      return Direction::NORTHWEST;
    } else {
      return Direction::SOUTHWEST;
    }
  } else if (diff.x == 0) {
    if (diff.y == 0) {
      return Direction::STATIONARY;
    } else if (diff.y > 0) {
      return Direction::NORTH;
    } else {
      return Direction::SOUTH;
    }
  } else {
    if (diff.y == 0) {
      return Direction::EAST;
    } else if (diff.y > 0) {
      return Direction::NORTHEAST;
    } else {
      return Direction::SOUTHEAST;
    }
  }
}
