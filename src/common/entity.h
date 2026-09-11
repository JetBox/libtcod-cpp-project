#pragma once

#include <libtcod/color.hpp>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "components/component.h"
#include "idGenerator.h"
#include "position.h"

enum class RenderOrder { Corpse = 0, Interactable = 1, Item = 2, Actor = 3 };

class Entity {
 public:
  Entity(
      Position pos,
      int ch,
      tcod::ColorRGBA color,
      std::string name,
      RenderOrder renderOrder,
      bool blocksMovement,
      bool blocksVision = false)
      : pos(pos),
        ch(ch),
        color(color),
        name(name),
        renderOrder(renderOrder),
        blocksMovement(blocksMovement),
        blocksVision(blocksVision) {}

  Entity() {
    pos = Position();
    ch = '?';
    color = {255, 255, 255, 255};
    name = "<Undefined>";
    blocksMovement = false;
    blocksVision = false;
  }

  Entity(int ch, tcod::ColorRGBA color, std::string name, bool blocksMovement)
      : ch(ch), color(color), name(name), blocksMovement(blocksMovement) {
    pos = Position();
  }

  RenderOrder getRenderOrder() const { return this->renderOrder; }
  void setRenderOrder(RenderOrder order) { this->renderOrder = order; }

  template <typename T, typename... Args>
  T& addComponent(Args&&... args) {
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    component->setEntity(this);
    T& ref = *component;
    components[std::type_index(typeid(T))] = std::move(component);
    return ref;
  }

  template <typename T>
  bool hasComponent() const {
    return components.count(std::type_index(typeid(T))) > 0;
  }

  template <typename T>
  void removeComponent() {
    components.erase(std::type_index(typeid(T)));
  }

  template <typename T>
  T& getComponent() {
    return *static_cast<T*>(components.at(std::type_index(typeid(T))).get());
  }

  std::string getName() { return this->name; }

  void move(Direction dir);

  void setPosition(Position p) { this->pos = p; }

  Position getPosition() { return this->pos; }

  void setChar(int c) { this->ch = c; }
  int getChar() { return this->ch; }

  tcod::ColorRGBA getColor() { return this->color; }

  bool getBlocksMovement() { return this->blocksMovement; }

  void setBlocksMovement(bool val) { this->blocksMovement = val; }

  void setBlocksVision(bool val) { this->blocksVision = val; }
  bool getBlocksVision() { return this->blocksVision; }

  IDGenerator::ID getID() { return this->id; }

  void setID(IDGenerator::ID id) { this->id = id; }

 private:
  IDGenerator::ID id = 0;
  Position pos;
  int ch;
  tcod::ColorRGBA color;
  std::string name;
  RenderOrder renderOrder = RenderOrder::Actor;
  bool blocksMovement;
  bool blocksVision;

  // Components
  std::unordered_map<std::type_index, std::unique_ptr<BaseComponent>> components;
};

// Particulars of Enemies
std::unique_ptr<Entity> spawnOrc(Position p);
std::unique_ptr<Entity> spawnTroll(Position p);
