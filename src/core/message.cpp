#include "message.h"

#include "windowFrame.h"

const std::string Message::getDisplayText() const {
  if (repeat > 0) {
    return this->messageText + "(x" + std::to_string(this->repeat) + ")";
  }
  return this->messageText;
}

void MessageLog::add(Message msg) {
  if (!messages.empty()) {
    Message& last = messages.back();
    if (last.messageText == msg.messageText && last.fg == msg.fg) {
      last.repeat++;
      return;
    }
  }
  this->messages.push_back(std::move(msg));
  if (this->messages.size() > this->maxMessages) {
    messages.pop_front();
  }
}

void MessageLog::render(tcod::Console* console, int x, int y, int width, int height) const {
  // Draw the frame and get the inner content rect back.
  WindowFrame frame(x, y, width, height, true);
  frame.setTitle("Messages");
  const std::array<int, 4> inner = frame.draw(*console);

  const int inner_x = inner[0];
  const int inner_y = inner[1];
  const int inner_width = inner[2];
  const int inner_height = inner[3];

  int y_bottom = inner_y + inner_height;  // one past the last row

  // newest first, walking upward
  for (auto it = this->messages.rbegin(); it != this->messages.rend(); ++it) {
    const int msg_height = tcod::get_height_rect(inner_width, it->messageText);
    y_bottom -= msg_height;
    if (y_bottom < inner_y) {
      // Partially or fully scrolled off the top; stop (or clip).
      break;
    }
    tcod::print_rect(
        *console, {inner_x, y_bottom, inner_width, msg_height}, it->getDisplayText(), it->fg, std::nullopt);
  }
}
