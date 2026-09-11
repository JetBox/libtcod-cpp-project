#include "alertBox.h"

#include "core/engine.h"
#include "core/windowFrame.h"

void AlertBox::render(Engine& /* engine*/, tcod::Console* console) {
  int w = 40;
  int h = 3;
  int x = (VIEW_WIDTH - w) / 2;
  int y = 2;

  WindowFrame frame = WindowFrame(x, y, w, h);
  frame.setTitle("ALERT");
  frame.setFooter("Press any key to exit.");
  auto [cx, cy, cw, ch] = frame.draw(*console);
  tcod::print_rect(*console, {cx, cy}, this->message, WHITE, std::nullopt);
}

bool AlertBox::handleEvent(Engine& /* engine */, SDL_Event* event) {
  if (event->type != SDL_EVENT_KEY_DOWN) {
    return true;
  }
  return false;
}
