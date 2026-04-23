#pragma once

#include "components/component.hpp"
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// A Screen is a named collection of Components with optional lifecycle hooks.
//   on_enter : called when this screen becomes active
//   on_update: called every frame BEFORE its components are updated
//   on_draw  : called every frame AFTER its components are drawn
class Screen {
public:
  std::vector<Component *> components;
  std::function<void()> on_enter;
  std::function<void()> on_update;
  std::function<void()> on_draw;

  void Add(Component *component) { components.push_back(component); }
  void Remove(Component *component) {
    components.erase(
        std::remove(components.begin(), components.end(), component),
        components.end());
  }

  void Enter() {
    if (on_enter)
      on_enter();
  }

  void Update() {
    if (on_update)
      on_update();
    for (Component *c : components) {
      c->Update();
    }
  }

  void Draw() const {
    for (Component *c : components) {
      c->Draw();
    }
    if (on_draw)
      on_draw();
  }
};

// ScreenManager owns nothing; it just routes Update/Draw to the active screen.
class ScreenManager {
private:
  std::unordered_map<std::string, Screen *> screens;
  std::string current;

public:
  void Register(const std::string &name, Screen *screen) {
    screens[name] = screen;
  }

  void SetActive(const std::string &name) {
    current = name;
    auto it = screens.find(current);
    if (it != screens.end() && it->second != nullptr) {
      it->second->Enter();
    }
  }

  const std::string &Active() const { return current; }

  Screen *Current() {
    auto it = screens.find(current);
    return it == screens.end() ? nullptr : it->second;
  }

  void Update() {
    if (Screen *s = Current()) {
      s->Update();
    }
  }

  void Draw() const {
    auto it = screens.find(current);
    if (it != screens.end() && it->second != nullptr) {
      it->second->Draw();
    }
  }
};
