#include "playState.h"

#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>

#include <memory>

#include "actions/autoExploreAction.h"
#include "actions/errorAction.h"
#include "actions/grabAction.h"
#include "actions/kickAction.h"
#include "actions/openAction.h"
#include "actions/unlockAction.h"
#include "actions/walkAction.h"
#include "common/glyphs.h"
#include "common/position.h"
#include "components/ai/aiComponent.h"
#include "components/combat.h"
#include "components/explorer.h"
#include "components/inventory.h"
#include "components/item.h"
#include "components/playerState.h"
#include "components/spells.h"
#include "components/vitals.h"
#include "core/engine.h"
#include "core/windowFrame.h"
#include "menus/inventoryMenu.h"
#include "states/menus/directionSelector.h"
#include "states/menus/inventoryMenu.h"
#include "states/menus/spellList.h"
#include "tileSelectionState.h"

void PlayState::update(Engine& engine) {
  while (true) {
    // Get all the actors (speed) and determine if any are ready
    std::vector<Entity*> eList = engine.getCurrentMap().getEntitiesWithComponent<Speed>();
    Entity* nextEntity = nullptr;
    int maxEnergy = 0;
    for (auto e : eList) {
      Speed s = e->getComponent<Speed>();
      if (s.canAct()) {
        if (s.getEnergy() > maxEnergy) {
          maxEnergy = s.getEnergy();
          nextEntity = e;
        }
      }
    }

    // Update speed if no nextEntity
    if (!nextEntity) {
      for (auto e : eList) {
        e->getComponent<Speed>().gainEnergy();
      }
      return;
    }

    // If there is a nextEntity, give a tie to the player if there's a perfect tie
    if (engine.getPlayer()->getComponent<Speed>().getEnergy() >= maxEnergy) {
      nextEntity = engine.getPlayer();
    }

    // If it's the player's turn but there's no queued action, stop and wait for input.
    if (nextEntity == engine.getPlayer() && engine.getQueuedActions().empty() &&
        engine.getPlayer()->getComponent<PlayerStateComponent>().getCurrentState() == PlayerState::PLAYING) {
      return;
    }

    // Process the turn of the next entity
    processEntityTurn(engine, nextEntity);

    // Process any deaths
    for (auto& e : engine.getCurrentMap().getEntitiesWithComponent<Fighter>()) {
      if (!e->getComponent<Fighter>().isAlive()) {
        e->clearEntity();
        e->setChar(CORPSE_GLYPH);
        e->addComponent<Item>(e->getName() + " corpse", "Corpse");
        e->setRenderOrder(RenderOrder::Corpse);
        e->setBlocksMovement(false);
      }
    }
  }
}

void PlayState::processEntityTurn(Engine& engine, Entity* entity) {
  std::unique_ptr<Action> action = nullptr;
  Entity* player = engine.getPlayer();

  if (entity == player) {
    PlayerState playerState = player->getComponent<PlayerStateComponent>().getCurrentState();
    if (playerState == PlayerState::AUTO_EXPLORE) {
      // If we see an enemy (any entity with a Fighter that isn't the Player), stop
      std::vector<Entity*> ve = visibleEntities(engine.getCurrentMap(), player->getComponent<Explorer>(), player);
      for (auto e : ve) {
        if (e->hasComponent<Fighter>()) {
          player->getComponent<PlayerStateComponent>().setState(PlayerState::PLAYING);
          player->getComponent<Explorer>().resetAutoExploreDestination();
          engine.getMessageLog().add(Message("You see a " + e->getName() + ", so you stop."));
          return;
        }
      }

      // Queue up the next tile to explore
      // TODO: handle grabbing items

      // If the Explorer's autoExploreDestination is -1,-1, then we need to recompute
      Explorer& explorer = player->getComponent<Explorer>();
      Position nextPositionTo = explorer.getAutoExploreDestination();
      if (!explorer.hasAutoExploreDestination()) {
        nextPositionTo = player->getComponent<Explorer>().getNextAutoExploreDestination(engine);
      }

      SDL_Log(
          "Player POS: (%d, %d) - AutoExplore To: (%d, %d)",
          player->getPosition().x,
          player->getPosition().y,
          nextPositionTo.x,
          nextPositionTo.y);
      if (nextPositionTo == player->getPosition()) {
        // We can't autoexplore anymore, go back to playing with a message
        player->getComponent<PlayerStateComponent>().setState(PlayerState::PLAYING);
        explorer.resetAutoExploreDestination();
        engine.getMessageLog().add(Message("You have explored everywhere you can see."));
        return;
      }
      Position nextStep = engine.getCurrentMap().getNextStep(player->getPosition(), nextPositionTo, player);
      Direction dir = getDirectionToPosition(player->getPosition(), nextStep);
      action = std::make_unique<AutoExploreAction>(dir);
    }
    if (playerState == PlayerState::PLAYING) {
      // Action comes from queuedActions
      std::deque<std::unique_ptr<Action>>& queue = engine.getQueuedActions();
      if (queue.empty()) {
        return;  // Wait for player input
      }
      action = std::move(queue.front());
      queue.pop_front();
    }

  } else {
    action = entity->getComponent<AIComponent>().getAction(engine);
  }

  // Try to perform the action repeatedly until success
  ActionResult result;
  while (true) {
    result = action->perform(engine, entity);
    SDL_Log(
        "%s - %d: Action performed: %d -- %p",
        entity->getName().c_str(),
        entity->getID(),
        result.succeeded,
        result.alternate.get());
    // If there's a message, add it to the log
    Message& msg = result.message;
    if (msg.messageText != "" && entity == engine.getPlayer()) {
      engine.getMessageLog().add(msg);
    }

    // If there's an alternate, perform that
    if (result.alternate) {
      action = std::move(result.alternate);
    } else {
      break;
    }
  }

  // If an AI failed, log and continue
  if (!result.succeeded && entity != engine.getPlayer()) {
    SDL_Log("Enemy %s with id %d had an unsuccessful action.", entity->getName().c_str(), entity->getID());
    entity->getComponent<Speed>().spend(result.energyCost);
    return;
  }

  if (result.succeeded) {
    // Update Speed
    if (entity->hasComponent<Speed>()) {
      entity->getComponent<Speed>().spend(result.energyCost);
    }

    // Update Vitals
    if (entity->hasComponent<Vitals>()) {
      entity->getComponent<Vitals>().updateTick();
    }

    // Update FOV for all Explorers
    std::vector<Entity*> explorers = engine.getCurrentMap().getEntitiesWithComponent<Explorer>();
    for (auto& e : explorers) {
      e->getComponent<Explorer>().updateFOV(engine.getCurrentMap());
    }
  }
}

bool PlayState::handleEvent(Engine& engine, SDL_Event* event) {
  // First, don't overqueue
  if (engine.getQueuedActions().size() > 0) {
    return true;
  }
  Entity* player = engine.getPlayer();

  // Switch on PlayerState
  PlayerState playerState = player->getComponent<PlayerStateComponent>().getCurrentState();
  if (playerState == PlayerState::AUTO_EXPLORE) {
    // All input should stop auto-explore
    if (event->type == SDL_EVENT_KEY_DOWN) {
      player->getComponent<Explorer>().resetAutoExploreDestination();
      player->getComponent<PlayerStateComponent>().setState(PlayerState::PLAYING);
      return true;
    }
  }

  // Game Event Processing
  if (event->type == SDL_EVENT_KEY_DOWN) {
    // Menu Processing

    // Spells
    if (event->key.key == SDLK_S) {
      if (!player->hasComponent<Spellbook>()) {
        engine.queueAction(
            std::make_unique<ErrorAction>("ERROR: Player does not have Spellbook to create Cast Spell Menu."));
        return true;
      }
      engine.pushState(std::make_unique<SpellList>());
      return true;
    }
    // Inventory
    if (event->key.key == SDLK_I) {
      if (!player->hasComponent<Inventory>()) {
        engine.queueAction(
            std::make_unique<ErrorAction>("ERROR: Player does not have Inventory to create Inventory Menu."));
        return true;
      }
      engine.pushState(
          std::make_unique<InventoryMenu>(
              "Inventory", player->getComponent<Inventory>().getItems(), inspectMenuOnChoose));
      return true;
    }
    // Wear
    if (event->key.key == SDLK_W) {
      if (!player->hasComponent<Inventory>()) {
        engine.queueAction(
            std::make_unique<ErrorAction>("ERROR: Player does not have Inventory to create Inventory Menu."));
        return true;
      }
      engine.pushState(
          std::make_unique<InventoryMenu>(
              "What do you equip?",
              player->getComponent<Inventory>().getItems(),
              wearMenuOnChoose,
              filterForEquippable));
      return true;
    }
    // Drop
    if (event->key.key == SDLK_D) {
      if (!player->hasComponent<Inventory>()) {
        engine.queueAction(
            std::make_unique<ErrorAction>("ERROR: Player does not have Inventory to create Inventory Menu."));
        return true;
      }
      engine.pushState(
          std::make_unique<InventoryMenu>(
              "What do you want to drop?", player->getComponent<Inventory>().getItems(), dropMenuOnChoose));
      return true;
    }
    // Open
    if (event->key.key == SDLK_O) {
      engine.pushState(std::make_unique<DirectionSelector>(openActionOnSelect, "Open"));
      return true;
    }
    // Unlock
    if (event->key.key == SDLK_U) {
      engine.pushState(std::make_unique<DirectionSelector>(unlockActionOnSelect, "Unlock"));
      return true;
    }
    // Kick
    if (event->key.key == SDLK_K) {
      engine.pushState(std::make_unique<DirectionSelector>(kickActionOnSelect, "Kick"));
      return true;
    }
    // Look
    if (event->key.key == SDLK_SEMICOLON) {
      engine.pushState(std::make_unique<TileSelectionState>(true, engine.getPlayer()->getPosition(), lookOnEnter));
      return true;
    }
    // Auto-Expxlore
    if (event->key.key == SDLK_KP_0) {
      engine.getPlayer()->getComponent<PlayerStateComponent>().setState(PlayerState::AUTO_EXPLORE);
      return true;
    }

    // Action Handling
    std::unique_ptr<Action> action = this->processKeyDown(event->key);
    if (action) {
      engine.queueAction(std::move(action));
    }
  }
  return true;
}

void PlayState::render(Engine& engine, tcod::Console* console) {
  engine.computeCamera();

  WindowFrame mapFrame(VIEW_X, VIEW_Y, VIEW_WIDTH, VIEW_HEIGHT, /*doubleFrame=*/true);
  mapFrame.draw(*console);

  engine.getCurrentMap().render(
      console, engine.getPlayer(), engine.getCamera(), MAP_VIEW_X, MAP_VIEW_Y, MAP_VIEW_HEIGHT, MAP_VIEW_WIDTH);

  engine.getMessageLog().render(console, TEXT_X, TEXT_Y, TEXT_WIDTH, TEXT_HEIGHT);
  engine.renderUI(console);
}

std::unique_ptr<Action> PlayState::processKeyDown(SDL_KeyboardEvent ev) {
  switch (ev.key) {
    // Walk Actions
    case SDLK_LEFT:
    case SDLK_KP_4:
      return std::make_unique<WalkAction>(Direction::WEST);
    case SDLK_RIGHT:
    case SDLK_KP_6:
      return std::make_unique<WalkAction>(Direction::EAST);
    case SDLK_UP:
    case SDLK_KP_8:
      return std::make_unique<WalkAction>(Direction::NORTH);
    case SDLK_DOWN:
    case SDLK_KP_2:
      return std::make_unique<WalkAction>(Direction::SOUTH);
    case SDLK_KP_7:
      return std::make_unique<WalkAction>(Direction::NORTHWEST);
    case SDLK_KP_9:
      return std::make_unique<WalkAction>(Direction::NORTHEAST);
    case SDLK_KP_1:
      return std::make_unique<WalkAction>(Direction::SOUTHWEST);
    case SDLK_KP_3:
      return std::make_unique<WalkAction>(Direction::SOUTHEAST);
    case SDLK_KP_5:
      return std::make_unique<WalkAction>(Direction::STATIONARY);

    // Other Actions
    case SDLK_G:
      return std::make_unique<GrabAction>();

    // Default
    default:
      return nullptr;  // no action for this key
  }
}
