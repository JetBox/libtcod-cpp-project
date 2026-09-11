#pragma once

#include <libtcod.hpp>
#include <libtcod/color.hpp>

static constexpr auto WHITE = tcod::ColorRGB{255, 255, 255};
static constexpr auto BLACK = tcod::ColorRGB{0, 0, 0};
static constexpr auto RED = tcod::ColorRGB{200, 0, 0};

static constexpr auto SHROUD_BLACK = tcod::ColorRGBA{0, 0, 0, 255};
static constexpr auto ENTITY_WHITE = tcod::ColorRGBA{255, 255, 255, 255};

static constexpr auto FLOOR_LIGHT = tcod::ColorRGBA{200, 180, 50, 255};
static constexpr auto FLOOR_DARK = tcod::ColorRGBA{50, 50, 150, 255};
static constexpr auto WALL_LIGHT = tcod::ColorRGBA{130, 110, 50, 255};
static constexpr auto WALL_DARK = tcod::ColorRGBA{0, 0, 100, 255};

static constexpr auto ORC_GREEN = tcod::ColorRGBA{63, 127, 63, 255};
static constexpr auto ORC_GREEN_DARK = tcod::ColorRGBA{20, 94, 35, 255};
static constexpr auto TROLL_GREEN = tcod::ColorRGBA{0, 127, 0, 255};
