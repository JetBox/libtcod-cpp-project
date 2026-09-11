#pragma once

#include <functional>

#include "component.h"
#include "core/engine.h"

class Openable : public BaseComponent {
 public:
  using ActionFn = std::function<void(Engine&, Entity*, Entity*)>;

  Openable(
      int closedCh,
      int openCh,
      bool passableOnOpen = false,
      bool blocksVisionOnClose = false,
      bool canBeClosed = true,
      bool closed = true,
      ActionFn onOpen = [](Engine&, Entity*, Entity*) { return; })
      : closedCh(closedCh),
        openCh(openCh),
        passableOnOpen(passableOnOpen),
        blocksVisionOnClose(blocksVisionOnClose),
        canBeClosed(canBeClosed),
        closed(closed),
        onOpen(onOpen) {}

  bool isClosed() { return this->closed; }
  void toggleOpen(Engine& engine, Entity* entity);

 private:
  int closedCh;
  int openCh;
  bool passableOnOpen = false;
  bool blocksVisionOnClose = false;
  bool canBeClosed = true;
  bool closed = true;
  ActionFn onOpen;
};
