#pragma once

#include <algorithm>
#include <cstdint>

class IDGenerator {
 public:
  using ID = std::uint32_t;
  ID next() { return this->counter++; }
  void observe(ID existing) { this->counter = std::max(counter, existing + 1); }
  ID peek() const { return counter; }

 private:
  ID counter = 1;  // an ID of 0 == invalid
};
