#pragma once

#include <deque>
#include <libtcod/color.hpp>
#include <string>

#include "common/colors.h"

struct Message {
  std::string messageText;
  tcod::ColorRGB fg;
  int repeat = 0;

  Message() {
    this->messageText = "";
    this->fg = WHITE;
    this->repeat = 0;
  }
  Message(std::string text, TCODColor fg = WHITE, int repeat = 0) : messageText(text), fg(fg), repeat(repeat) {}

  const std::string getDisplayText() const;
};

class MessageLog {
 public:
  MessageLog() = default;

  void add(Message msg);

  void render(tcod::Console* console, int x, int y, int width, int height) const;

  std::deque<Message>& getMessages() { return messages; }

 private:
  std::deque<Message> messages;
  static constexpr std::size_t maxMessages = 100;
};
