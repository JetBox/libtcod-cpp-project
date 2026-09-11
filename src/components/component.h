#pragma once

class Entity;

class BaseComponent {
 public:
  BaseComponent() = default;
  virtual ~BaseComponent() = default;

  void setEntity(Entity* e) { this->owner = e; }
  Entity* getEntity() { return this->owner; }

 private:
  Entity* owner = nullptr;
};
