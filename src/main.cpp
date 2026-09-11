#include <libtcod/color.hpp>
#include <libtcod/tileset.hpp>
#include <memory>

#include "components/explorer.h"

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <libtcod.hpp>

#include "components/body.h"
#include "components/combat.h"
#include "components/inventory.h"
#include "components/vitals.h"
#include "core/engine.h"

/// Return the data directory.
auto get_data_dir() -> std::filesystem::path {
  static auto root_directory = std::filesystem::path{"."};  // Begin at the working directory.
  while (!std::filesystem::exists(root_directory / "data")) {
    // If the current working directory is missing the data dir then it will assume it exists in any parent directory.
    root_directory /= "..";
    if (!std::filesystem::exists(root_directory)) {
      throw std::runtime_error("Could not find the data directory.");
    }
  }
  return root_directory / "data";
};

static tcod::Console g_console;  // The global console object.
static tcod::Context g_context;  // The global libtcod context.

Engine gameEngine;

// Called every frame
SDL_AppResult SDL_AppIterate(void*) {
  gameEngine.update();
  gameEngine.render(&g_console, &g_context);
  return SDL_APP_CONTINUE;
}

// Handle events
SDL_AppResult SDL_AppEvent(void*, SDL_Event* event) { return gameEngine.handleEvent(event); }

// Main entry point
SDL_AppResult SDL_AppInit(void**, int argc, char** argv) {
  auto params = TCOD_ContextParams{};
  params.argc = argc;
  params.argv = argv;
  // params.renderer_type = TCOD_RENDERER_SDL2;
  params.renderer_type = TCOD_RENDERER_SDL;
  params.vsync = 1;
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.window_title = "Libtcod C++ Project";  // TODO: Change this

  SDL_SetLogPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

  auto tileset_dejavu = tcod::load_tilesheet(get_data_dir() / "dejavu16x16_gs_tc.png", {32, 8}, tcod::CHARMAP_TCOD);
  auto tileset_terminal =
      tcod::load_tilesheet(get_data_dir() / "terminal12x12_gs_ro.png", {16, 16}, tcod::CHARMAP_CP437);
  params.tileset = tileset_terminal.get();

  g_console = tcod::Console{SCREEN_WIDTH, SCREEN_HEIGHT};
  params.console = g_console.get();

  g_context = tcod::Context(params);

  auto player = std::make_unique<Entity>(
      Position((SCREEN_WIDTH / 2), SCREEN_HEIGHT / 2),
      '@',
      tcod::ColorRGBA(255, 255, 255, 255),
      "player",
      RenderOrder::Actor,
      true);
  player->addComponent<Body>();
  player->addComponent<Inventory>();
  player->addComponent<Fighter>();
  player->addComponent<BaseStats>(5, 5, 5, 5, 5);
  player->addComponent<Vitals>(100, 0);
  player->addComponent<Speed>();
  player->addComponent<Explorer>(player.get());
  gameEngine.setPlayer(std::move(player));

  return SDL_APP_CONTINUE;
}
// Called before existing
void SDL_AppQuit(void*, SDL_AppResult) {}

/* AI Roadmap
- Define Basic AIBehaviors (modes that decide the next action)
  - Move
  - Hunt
  - Flee
  - Rest
- Define Basic AIGoals (things the AI is trying to achieve; each decides which AIBehavior to use depending on state)
  - Eat
  - Sleep
  - Explore
- Define algorithm for AIComponent that Sorts Goals based on Needs (Vitals); factor in time spent on goal
- Create extensible Pathfinding strategies (dumb vs smart; use enum?)
-
*/

/*
TODO LIST:
- Enemies
  - Behaviors
  - Pathfinding
  - Actual Combat
- Traps (Chests and Doors)
- View equipment
- Consumables (potions, food, scrolls)
- Ranged combat
- Auto-Explore
- Magic
- Config Files
*/
