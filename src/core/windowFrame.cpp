#include "windowFrame.h"

#include "common/colors.h"
namespace {
constexpr std::array<int, 9> DOUBLE_FRAME = {
    0x2554,
    0x2550,
    0x2557,  // ╔ ═ ╗
    0x2551,
    ' ',
    0x2551,  // ║   ║
    0x255A,
    0x2550,
    0x255D,  // ╚ ═ ╝
};

constexpr std::array<int, 9> SINGLE_FRAME = {
    0x250C,
    0x2500,
    0x2510,  // ┌ ─ ┐
    0x2502,
    ' ',
    0x2502,  // │   │
    0x2514,
    0x2500,
    0x2518,  // └ ─ ┘
};
}  // namespace

std::array<int, 4> WindowFrame::draw(tcod::Console& console) const {
  // Fill the interior
  tcod::draw_rect(console, {this->x, this->y, this->width, this->height}, ' ', WHITE, BLACK);

  // Draw border
  std::array<int, 9> frame = this->doubleFrame ? DOUBLE_FRAME : SINGLE_FRAME;
  tcod::draw_frame(console, {this->x, this->y, this->width, this->height}, frame, WHITE, BLACK, TCOD_BKGND_SET, false);

  // Draw Title
  if (this->title) {
    const std::string label = " " + *(this->title) + " ";
    tcod::print_rect(console, {this->x, this->y, this->width, 1}, label, BLACK, WHITE, TCOD_CENTER);
  }

  // Draw Footer
  if (this->footer) {
    tcod::print_rect(
        console, {this->x, this->y + this->height - 1, this->width, 1}, *(this->footer), WHITE, BLACK, TCOD_CENTER);
  }

  // Return inner content rect
  return {this->x + 1, this->y + 1, this->width - 2, this->height - 2};
}
