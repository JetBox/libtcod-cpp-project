#include "errorAction.h"

#include "action.h"

ActionResult ErrorAction::perform(Engine& /* engine */, Entity* /* entity */) {
  return ActionResult(false, this->errorMessage);
}
