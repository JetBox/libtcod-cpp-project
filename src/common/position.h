#pragma once

#include <array>
#include <stdexcept>

enum class Direction { NORTH, SOUTH, EAST, WEST, NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST };

struct Position {
  int x, y;

  Position(int x, int y) : x(x), y(y) {}

  Position() = default;

  std::array<int, 2> const getAsArray() { return {this->x, this->y}; }

  bool operator==(const Position& other) const {
    if (this->x == other.x && this->y == other.y) {
      return true;
    }
    return false;
  }

  Position operator+(const Direction& dir) const {
    switch (dir) {
      case Direction::NORTH:
        return Position(this->x, this->y - 1);
      case Direction::EAST:
        return Position(this->x + 1, this->y);
      case Direction::WEST:
        return Position(this->x - 1, this->y);
        ;
      case Direction::SOUTH:
        return Position(this->x, this->y + 1);
      case Direction::NORTHEAST:
        return Position(this->x + 1, this->y - 1);
      case Direction::NORTHWEST:
        return Position(this->x - 1, this->y - 1);
      case Direction::SOUTHEAST:
        return Position(this->x + 1, this->y + 1);
      case Direction::SOUTHWEST:
        return Position(this->x - 1, this->y + 1);
      default:
        throw std::invalid_argument("Unexpected value of Direction");
    };
  }

  Position operator+(const Position& other) const { return Position(this->x + other.x, this->y + other.y); }

  Position operator-(const Direction& dir) const {
    switch (dir) {
      case Direction::NORTH:
        return Position(this->x, this->y + 1);
      case Direction::EAST:
        return Position(this->x - 1, this->y);
      case Direction::WEST:
        return Position(this->x + 1, this->y);
        ;
      case Direction::SOUTH:
        return Position(this->x, this->y - 1);
      case Direction::NORTHEAST:
        return Position(this->x - 1, this->y + 1);
      case Direction::NORTHWEST:
        return Position(this->x + 1, this->y + 1);
      case Direction::SOUTHEAST:
        return Position(this->x - 1, this->y - 1);
      case Direction::SOUTHWEST:
        return Position(this->x + 1, this->y - 1);
    };
  }
  Position operator-(const Position& other) const { return Position(this->x - other.x, this->y - other.y); }

  Position& operator+=(const Position& other) {
    *this = *this + other;
    return *this;
  }

  Position& operator+=(const Direction& dir) {
    *this = *this + dir;
    return *this;
  }

  Position& operator-=(const Position& other) {
    *this = *this - other;
    return *this;
  }

  Position& operator-=(const Direction& dir) {
    *this = *this - dir;
    return *this;
  }
};
