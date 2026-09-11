#include "engine.h"

#include <SDL3/SDL_events.h>
#include <libtcod/console.h>
#include <libtcod/fov_types.h>

#include <iostream>
#include <libtcod/console_printing.hpp>
#include <memory>
#include <string>

#include "SDL3/SDL_events.h"
#include "common/entity.h"
#include "common/idGenerator.h"
#include "common/position.h"
#include "components/combat.h"
#include "components/explorer.h"
#include "components/vitals.h"
#include "gen/procgen.h"
#include "states/playState.h"
#include "windowFrame.h"

Engine::Engine() {
  this->player = nullptr;
  this->rng = TCODRandom::getInstance();
  this->messageLog = MessageLog();
  this->gameWorld = GameWorld();
  this->camera = Position();
  this->states.push_back(std::make_unique<PlayState>());

  IDGenerator::ID mapID = this->gameWorld.createMap(MAP_WIDTH, MAP_HEIGHT);
  this->gameWorld.setActiveMap(mapID);

  generateDungeon(
      this->gameWorld, mapID, MAX_ROOMS, MIN_ROOM_SIZE, MAX_ROOM_SIZE, MAX_MONSTERS_PER_ROOM, MAX_ITEMS_PER_ROOM);
}

void Engine::update() {
  GameState* top = this->topState();
  if (top) {
    top->update(*this);
  }
}

void Engine::setPlayer(std::unique_ptr<Entity> e) {
  GameMap& map = this->gameWorld.getActiveMap();
  this->player = e.get();
  this->player->setPosition(map.getOrigin());
  this->gameWorld.addEntityToMap(std::move(e));

  this->player->getComponent<Explorer>().addMap(this->getCurrentMap());
  this->player->getComponent<Explorer>().updateFOV(this->getCurrentMap());
}

SDL_AppResult Engine::handleEvent(SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  GameState* top = this->topState();
  if (top) {
    bool keep = top->handleEvent(*this, event);
    if (!keep) {
      this->popState();
    }
  }

  return SDL_APP_CONTINUE;
}

void Engine::render(tcod::Console* console, tcod::Context* context) {
  console->clear();

  // Find lowest state we need to start drawing from
  int start = static_cast<int>(this->states.size()) - 1;
  while (start > 0 && this->states[start]->renderStateBelow()) {
    --start;
  }
  for (int i = start; i < static_cast<int>(this->states.size()); i++) {
    this->states[i]->render(*this, console);
  }

  context->present(*console);
}

void Engine::computeCamera() {
  int camX = this->player->getPosition().x - MAP_VIEW_WIDTH / 2;
  int camY = this->player->getPosition().y - MAP_VIEW_HEIGHT / 2;

  // Clamp so the viewport never shows outside the map.
  camX = std::max(0, std::min(camX, MAP_WIDTH - MAP_VIEW_WIDTH));
  camY = std::max(0, std::min(camY, MAP_HEIGHT - MAP_VIEW_HEIGHT));

  this->camera = {camX, camY};
}

void Engine::pushState(std::unique_ptr<GameState> state) { this->states.push_back(std::move(state)); }

void Engine::popState() {
  if (!this->states.empty()) {
    this->states.pop_back();
  }
}

GameState* Engine::topState() { return this->states.empty() ? nullptr : this->states.back().get(); }

void Engine::queueAction(std::unique_ptr<Action> act) { this->queuedActions.push_back(std::move(act)); }

void Engine::renderUI(tcod::Console* console) {
  // Border
  WindowFrame frame(UI_X, 0, UI_WIDTH, UI_HEIGHT, /*doubleFrame=*/true);
  frame.draw(*console);

  // Draw at each line
  int w = UI_WIDTH - 2;
  int inset = UI_X + 1;
  int line = 1;

  // Draw Name and Title TODO: Add Title/Identity
  tcod::print_rect(*console, {inset, line, w, 1}, this->getPlayer()->getName(), WHITE, std::nullopt);

  // Draw Health
  line += 2;
  if (!this->getPlayer()->hasComponent<Fighter>()) {
    std::cerr << "ERROR: Player doesn't have Fighter." << std::endl;
  }
  Fighter f = this->getPlayer()->getComponent<Fighter>();
  std::string stringHP = " HP: " + std::to_string(f.getHP()) + " / " + std::to_string(f.getMaxHP());
  tcod::print_rect(*console, {inset, line++, w, 1}, stringHP, WHITE, std::nullopt);

  // Draw Vitals
  line++;
  if (!this->getPlayer()->hasComponent<Vitals>()) {
    std::cerr << "ERROR: Player doesn't have Vitals." << std::endl;
  }
  Vitals v = this->getPlayer()->getComponent<Vitals>();
  std::string stringHunger = "HUN: " + std::to_string(v.getHunger()) + " / " + std::to_string(Vitals::MAX_HUNGER);
  std::string stringFatigue = "FAT: " + std::to_string(v.getFatigue()) + " / " + std::to_string(Vitals::MAX_FATIGUE);
  tcod::print_rect(*console, {inset, line++, w, 1}, stringHunger, WHITE, std::nullopt);
  tcod::print_rect(*console, {inset, line++, w, 1}, stringFatigue, WHITE, std::nullopt);
}
