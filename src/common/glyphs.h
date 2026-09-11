#pragma once

#include <string>

// static constexpr int UNCHECKED_BOX = 0x2610;
static constexpr int UNOPENED_CHEST = 0x00C7;
static constexpr int OPENED_CHEST = 0x00FC;
static constexpr int OPEN_CIRCLE = ' ';
static constexpr int FILLED_CIRCLE = 0x2022;
static constexpr int DOOR_CHAR = '#';
static constexpr int DOOR_OPEN_CHAR = '/';
static constexpr int KEY_GLYPH = 0x266A;

static const std::string OPEN_CIRCLE_STRING = "\u25CB";
static const std::string FILLED_CIRCLE_STRING = "\u25C9";

inline std::string utf8(int codepoint) {
  std::string s;
  if (codepoint < 0x80) {
    s += static_cast<char>(codepoint);
  } else if (codepoint < 0x800) {
    s += static_cast<char>(0xC0 | (codepoint >> 6));
    s += static_cast<char>(0x80 | (codepoint & 0x3F));
  } else if (codepoint < 0x10000) {
    s += static_cast<char>(0xE0 | (codepoint >> 12));
    s += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
    s += static_cast<char>(0x80 | (codepoint & 0x3F));
  } else {
    s += static_cast<char>(0xF0 | (codepoint >> 18));
    s += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
    s += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
    s += static_cast<char>(0x80 | (codepoint & 0x3F));
  }
  return s;
}
