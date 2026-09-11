#include "openable.h"

#include "common/entity.h"

void Openable::toggleOpen(Engine& engine, Entity* entity) {
  if (this->closed) {
    this->closed = false;
    this->getEntity()->setBlocksMovement(!this->passableOnOpen);
    this->getEntity()->setChar(this->openCh);
    this->getEntity()->setBlocksVision(false);
    this->onOpen(engine, entity, this->getEntity());
  } else if (this->canBeClosed) {
    this->closed = true;
    this->getEntity()->setBlocksMovement(true);
    this->getEntity()->setChar(this->closedCh);
    this->getEntity()->setBlocksVision(this->blocksVisionOnClose);
  }
  engine.getCurrentMap().updateFOVCell(this->getEntity()->getPosition());
}
