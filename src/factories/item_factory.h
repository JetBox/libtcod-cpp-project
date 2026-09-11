#pragma once

#include <libtcod/mersenne.hpp>
#include <memory>
#include <string>
#include <vector>

#include "common/entity.h"
#include "common/position.h"
#include "components/body.h"

// Naive randomizer
static std::vector<BodyPartKind> kinds = {
    BodyPartKind::HEAD, BodyPartKind::ARMS, BodyPartKind::TORSO, BodyPartKind::LEGS, BodyPartKind::FEET};

static std::vector<std::string> adjectives = {
    "sturdy", "rusty", "tarnished", "shiny", "glossy", "scratched", "pristine", "decrepit", "iron", "copper"};

std::string getEquipName(BodyPartKind kind);
std::string getAdjective(TCODRandom* rng);
BodyPartKind getBodyPartKind(TCODRandom* rng);
char getEquipItemChar(BodyPartKind kind);

std::unique_ptr<Entity> getRandomEquipment(TCODRandom* rng, Position p);
